#include <SFML/Graphics.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <chess.hpp>
#include <deque>
#include <thread>
#include <functional>
#include <mutex>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;
using json = nlohmann::json;

static std::string sq2uci(chess::Square s) {
    return { char('a' + int(s.file())), char('1' + int(s.rank())) };
}
static chess::Square uci2sq(const std::string& s) {
    return chess::Square(chess::File(s[0] - 'a'), chess::Rank(s[1] - '1'));
}
static std::string pieceChar(chess::Piece p) {
    if (p == chess::Piece::NONE) return "";
    bool w = (p.color() == chess::Color::WHITE);
    auto t = p.type();
    if (t == chess::PieceType::PAWN)   return w ? "P" : "p";
    if (t == chess::PieceType::KNIGHT) return w ? "N" : "n";
    if (t == chess::PieceType::BISHOP) return w ? "B" : "b";
    if (t == chess::PieceType::ROOK)   return w ? "R" : "r";
    if (t == chess::PieceType::QUEEN)  return w ? "Q" : "q";
    if (t == chess::PieceType::KING)   return w ? "K" : "k";
    return "";
}

class NetClient {
public:
    NetClient()
        : work_(boost::asio::make_work_guard(io_)),
          game_sock_(io_), chat_sock_(io_)
    {
        thr_ = std::thread([this]{ io_.run(); });
    }
    ~NetClient() {
        boost::system::error_code ec;
        game_sock_.close(ec);
        chat_sock_.close(ec);
        io_.stop();
        if (thr_.joinable()) thr_.join();
    }

    void connect_game(const std::string& host, unsigned short port,
                      std::function<void(json)> cb,
                      std::function<void()> on_connected = nullptr) {
        on_game_ = std::move(cb);
        tcp::resolver r(io_);
        auto eps = r.resolve(host, std::to_string(port));
        boost::asio::async_connect(game_sock_, eps,
            [this, on_connected](auto ec, auto) {
                if (ec) { std::cerr << "connect_game: " << ec.message() << "\n"; return; }
                read_game();
                if (on_connected) on_connected();
            });
    }
    void connect_chat(const std::string& host, unsigned short port,
                      std::function<void(json)> cb,
                      std::function<void()> on_connected = nullptr) {
        on_chat_ = std::move(cb);
        tcp::resolver r(io_);
        auto eps = r.resolve(host, std::to_string(port));
        boost::asio::async_connect(chat_sock_, eps,
            [this, on_connected](auto ec, auto) {
                if (ec) { std::cerr << "connect_chat: " << ec.message() << "\n"; return; }
                read_chat();
                if (on_connected) on_connected();
            });
    }

    void send_game(json j) {
        std::string line = j.dump() + "\n";
        boost::system::error_code ec;
        boost::asio::write(game_sock_, boost::asio::buffer(line), ec);
        if (ec) std::cerr << "[cli] send_game error: " << ec.message() << "\n";
    }
    void send_chat(json j) {
        std::string line = j.dump() + "\n";
        boost::system::error_code ec;
        boost::asio::write(chat_sock_, boost::asio::buffer(line), ec);
        if (ec) std::cerr << "[cli] send_chat error: " << ec.message() << "\n";
    }

private:
    void read_game() {
        boost::asio::async_read_until(game_sock_,
            boost::asio::dynamic_buffer(gameReadBuf_), '\n',
            [this](auto ec, size_t) {
                if (ec) return;
                size_t pos = gameReadBuf_.find('\n');
                std::string line = gameReadBuf_.substr(0, pos);
                gameReadBuf_.erase(0, pos + 1);
                if (!line.empty() && line.back() == '\r') line.pop_back();
                auto j = json::parse(line, nullptr, false);
                if (!j.is_discarded() && on_game_) on_game_(j);
                read_game();
            });
    }
    void read_chat() {
        boost::asio::async_read_until(chat_sock_,
            boost::asio::dynamic_buffer(chatReadBuf_), '\n',
            [this](auto ec, size_t) {
                if (ec) return;
                size_t pos = chatReadBuf_.find('\n');
                std::string line = chatReadBuf_.substr(0, pos);
                chatReadBuf_.erase(0, pos + 1);
                if (!line.empty() && line.back() == '\r') line.pop_back();
                auto j = json::parse(line, nullptr, false);
                if (!j.is_discarded() && on_chat_) on_chat_(j);
                read_chat();
            });
    }

    boost::asio::io_context io_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
    tcp::socket game_sock_;
    tcp::socket chat_sock_;
    std::thread thr_;
    std::function<void(json)> on_game_, on_chat_;
    std::string gameReadBuf_, chatReadBuf_;
};

enum class AuthState { NotAuthorized, Waiting, Authorized };

struct Button {
    sf::FloatRect rect;
    std::string label;
    sf::Color color;
    bool hit(int mx, int my) const {
        return rect.contains({float(mx), float(my)});
    }
};

int main() {
    const int TILE = 80;
    const int PANEL = 300;
    const int FORM_W = 400;
    const int FORM_H = 320;

    sf::RenderWindow win(sf::VideoMode({unsigned(TILE*8 + PANEL), unsigned(TILE*8)}),
                         "Chess", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    bool fontOk = false;
    for (const char* p : {
            "assets/DejaVuSans.ttf", "../assets/DejaVuSans.ttf",
            "../../assets/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf" }) {
        if (font.openFromFile(p)) { fontOk = true; break; }
    }
    if (!fontOk) { std::cerr << "Cannot load font\n"; return 1; }

    chess::Board board;
    chess::Square sel = chess::Square::NO_SQ;
    std::vector<chess::Square> legalTargets;
    bool myTurn = false;
    bool iAmWhite = true;
    std::string opp = "-";
    std::string status = "Connect...";
    int myRating = 0;

    std::mutex uiMtx;
    std::deque<std::string> chatLog;
    std::string chatInput;

    AuthState authState = AuthState::NotAuthorized;
    std::string login, password;
    std::string authStatus = "Enter login and password";
    int activeField = 0;

    NetClient net;

    const float PANEL_X = float(TILE * 8) + 10.f;
    Button btnQueue  { sf::FloatRect{{PANEL_X, 200.f}, {130.f, 32.f}},
                       "Queue",  sf::Color(60,120,200) };
    Button btnResign { sf::FloatRect{{PANEL_X + 140.f, 200.f}, {130.f, 32.f}},
                       "Resign", sf::Color(200,60,60) };
    float formX = float(TILE*8 + PANEL) / 2.f - FORM_W / 2.f;
    float formY = float(TILE*8) / 2.f - FORM_H / 2.f;
    Button btnRegister { sf::FloatRect{{formX + 20.f,  formY + 240.f}, {160.f, 40.f}},
                         "Register", sf::Color(60,120,200) };
    Button btnLogin    { sf::FloatRect{{formX + 200.f, formY + 240.f}, {160.f, 40.f}},
                         "Login",    sf::Color(60,160,80)  };

    auto handle_game = [&](json j) {
        std::lock_guard lk(uiMtx);
        auto t = j.value("type", "");
        if (t == "ok") {
            myRating = j["rating"];
            status = "Online";
            authState = AuthState::Authorized;
            authStatus = "Welcome!";
            // сообщить чат-серверу свой логин
            net.send_chat({{"type","hello"},{"login",login}});
        } else if (t == "error") {
            std::string msg = j["msg"].get<std::string>();
            if (authState != AuthState::Authorized) {
                if (msg == "login exists" && !login.empty() && !password.empty()) {
                    net.send_game({{"type","login"},{"login",login},{"pass",password}});
                    authState = AuthState::Waiting;
                    authStatus = "User exists, logging in...";
                } else {
                    authState = AuthState::NotAuthorized;
                    authStatus = "Error: " + msg;
                }
            } else {
                status = "Error: " + msg;
            }
        } else if (t == "matched") {
            iAmWhite = j["white"];
            opp      = j["opp"];
            status   = "Match!";
            myTurn   = iAmWhite;
            board    = chess::Board();
            sel      = chess::Square::NO_SQ;
            legalTargets.clear();
        } else if (t == "move") {
            auto f  = uci2sq(j["from"].get<std::string>());
            auto to = uci2sq(j["to"].get<std::string>());
            chess::Move mv = chess::Move::make(f, to);
            if (board.isLegal(mv)) board.makeMove(mv);
            myTurn = true;
            sel = chess::Square::NO_SQ;
            legalTargets.clear();
        } else if (t == "game_over") {
            int newRating = j["rating"].get<int>();
            myRating = newRating;
            status = "Game over: " + j["result"].get<std::string>() +
                     " (rating " + std::to_string(newRating) + ")";
            myTurn = false;
            opp = "-";
            sel = chess::Square::NO_SQ;
            legalTargets.clear();
        }
    };
    auto handle_chat = [&](json j) {
        std::lock_guard lk(uiMtx);
        if (j.value("type", "") != "chat") return;
        std::string from = j.value("from", "");
        std::string to   = j.value("to", "");

        // Показываем сообщения, адресованные нам (или от нас)
        if (from != opp && from != login) return;
        if (to != login && to != opp) return;

        chatLog.push_back(from + ": " + j["text"].get<std::string>());
        if (chatLog.size() > 20) chatLog.pop_front();
    };

    net.connect_chat("127.0.0.1", 5556, handle_chat);
    net.connect_game("127.0.0.1", 5555, handle_game, [](){});

    auto drawButton = [&](const Button& b) {
        sf::RectangleShape r(b.rect.size);
        r.setPosition(b.rect.position);
        r.setFillColor(b.color);
        win.draw(r);
        sf::Text t(font, b.label, 18);
        t.setPosition({b.rect.position.x + 12.f, b.rect.position.y + 6.f});
        t.setFillColor(sf::Color::White);
        win.draw(t);
    };

    auto drawForm = [&]() {
        sf::RectangleShape bg({float(FORM_W), float(FORM_H)});
        bg.setFillColor(sf::Color(40, 40, 50));
        bg.setPosition({formX, formY});
        win.draw(bg);

        auto put = [&](const std::string& s, float x, float y, unsigned sz, sf::Color c) {
            sf::Text t(font, s, sz);
            t.setPosition({x, y});
            t.setFillColor(c);
            win.draw(t);
        };
        float bx = formX, by = formY;
        put("Login:", bx + 20, by + 20, 20, sf::Color::White);
        put(login + (activeField == 0 ? "_" : ""), bx + 20, by + 45, 22,
            activeField == 0 ? sf::Color::Yellow : sf::Color::White);
        put("Password:", bx + 20, by + 85, 20, sf::Color::White);
        std::string masked(password.size(), '*');
        put(masked + (activeField == 1 ? "_" : ""), bx + 20, by + 110, 22,
            activeField == 1 ? sf::Color::Yellow : sf::Color::White);
        put(authStatus, bx + 20, by + 160, 16, sf::Color::Cyan);
        put("Tab - switch field", bx + 20, by + 190, 14, sf::Color(180,180,180));
        drawButton(btnRegister);
        drawButton(btnLogin);
    };

    auto drawBoard = [&]() {
        std::string pieces[8][8];
        {
            std::lock_guard lk(uiMtx);
            for (int r = 0; r < 8; ++r)
                for (int f = 0; f < 8; ++f)
                    pieces[r][f].clear();
            for (int r = 0; r < 8; ++r) {
                for (int f = 0; f < 8; ++f) {
                    auto sq = chess::Square(chess::File(f), chess::Rank(r));
                    auto p  = board.at(sq);
                    if (p == chess::Piece::NONE) continue;
                    pieces[7 - r][f] = pieceChar(p);
                }
            }
        }

        win.clear(sf::Color(30, 30, 40));
        for (int r = 0; r < 8; ++r) {
            for (int f = 0; f < 8; ++f) {
                sf::RectangleShape tile({float(TILE), float(TILE)});
                tile.setPosition({float(f*TILE), float(r*TILE)});
                bool dark = (r + f) % 2;
                tile.setFillColor(dark ? sf::Color(120, 80, 60)
                                       : sf::Color(220, 200, 160));
                if (sel != chess::Square::NO_SQ) {
                    auto selSq = chess::Square(chess::File(f), chess::Rank(7 - r));
                    if (sel == selSq) tile.setFillColor(sf::Color(80, 180, 80));
                }
                win.draw(tile);
                if (!pieces[r][f].empty()) {
                    sf::Text txt(font, pieces[r][f], 60);
                    txt.setPosition({float(f*TILE + 14), float(r*TILE + 2)});
                    txt.setFillColor((pieces[r][f][0] >= 'A' && pieces[r][f][0] <= 'Z')
                                     ? sf::Color::White : sf::Color::Black);
                    win.draw(txt);
                }
            }
        }

        for (auto& t : legalTargets) {
            int fx = int(t.file());
            int fr = int(t.rank());
            int fy = 7 - fr;
            auto p = board.at(t);
            if (p != chess::Piece::NONE) {
                sf::CircleShape ring(float(TILE) * 0.42f);
                ring.setOrigin({float(TILE) * 0.42f, float(TILE) * 0.42f});
                ring.setPosition({float(fx*TILE + TILE/2), float(fy*TILE + TILE/2)});
                ring.setFillColor(sf::Color::Transparent);
                ring.setOutlineColor(sf::Color(220, 60, 60, 220));
                ring.setOutlineThickness(6.f);
                win.draw(ring);
            } else {
                sf::CircleShape dot(float(TILE) * 0.15f);
                dot.setOrigin({float(TILE) * 0.15f, float(TILE) * 0.15f});
                dot.setPosition({float(fx*TILE + TILE/2), float(fy*TILE + TILE/2)});
                dot.setFillColor(sf::Color(60, 220, 60, 200));
                win.draw(dot);
            }
        }

        float px = PANEL_X;
        auto put = [&](const std::string& s, float y, unsigned sz, sf::Color c) {
            sf::Text t(font, s, sz);
            t.setPosition({px, y});
            t.setFillColor(c);
            win.draw(t);
        };
        put("Login:  " + login, 10, 18, sf::Color::White);
        put("Rating: " + std::to_string(myRating), 40, 18, sf::Color::White);
        put("Opp:    " + (opp.empty() ? "-" : opp), 70, 18, sf::Color::White);
        put("Status: " + status, 100, 16, sf::Color::Cyan);
        put(iAmWhite ? "You: WHITE" : "You: BLACK", 130, 16, sf::Color::Yellow);
        put("CHAT:", 160, 18, sf::Color::Yellow);
        drawButton(btnQueue);
        drawButton(btnResign);

        {
            std::lock_guard lk(uiMtx);
            float y = 245;
            for (auto& m : chatLog) {
                bool isMine = (m.rfind(login + ":", 0) == 0);
                put(m, y, 14, isMine ? sf::Color(120, 220, 120) : sf::Color::White);
                y += 20;
            }
        }
        put("> " + chatInput + "_", float(TILE*8 - 30), 16, sf::Color::Green);
    };

    while (win.isOpen()) {
        while (auto e = win.pollEvent()) {
            if (e->is<sf::Event::Closed>()) { win.close(); break; }

            if (authState != AuthState::Authorized) {
                if (auto* te = e->getIf<sf::Event::TextEntered>()) {
                    if (te->unicode == 8) {
                        std::string& field = (activeField == 0 ? login : password);
                        if (!field.empty()) field.pop_back();
                    } else if (te->unicode == '\t') {
                        activeField = 1 - activeField;
                    } else if (te->unicode >= 32 && te->unicode < 127) {
                        std::string& field = (activeField == 0 ? login : password);
                        field += char(te->unicode);
                    }
                }
                if (auto* me = e->getIf<sf::Event::MouseButtonPressed>()) {
                    if (me->button == sf::Mouse::Button::Left) {
                        int mx = me->position.x, my = me->position.y;
                        if (btnRegister.hit(mx, my) && !login.empty() && !password.empty()) {
                            net.send_game({{"type","register"},{"login",login},{"pass",password}});
                            authState = AuthState::Waiting;
                            authStatus = "Registering...";
                        } else if (btnLogin.hit(mx, my) && !login.empty() && !password.empty()) {
                            net.send_game({{"type","login"},{"login",login},{"pass",password}});
                            authState = AuthState::Waiting;
                            authStatus = "Logging in...";
                        }
                    }
                }
            } else {
                if (auto* te = e->getIf<sf::Event::TextEntered>()) {
                    if (te->unicode == 8 && !chatInput.empty()) chatInput.pop_back();
                    else if (te->unicode >= 32 && te->unicode < 127)
                        chatInput += char(te->unicode);
                }
                if (auto* ke = e->getIf<sf::Event::KeyPressed>()) {
                    if (ke->code == sf::Keyboard::Key::Enter && !chatInput.empty()) {
                        net.send_chat({{"type","chat"},
                                       {"from",login},
                                       {"to",opp},
                                       {"text",chatInput}});
                        chatInput.clear();
                    }
                }
                if (auto* me = e->getIf<sf::Event::MouseButtonPressed>()) {
                    if (me->button == sf::Mouse::Button::Left) {
                        int mx = me->position.x, my = me->position.y;
                        if (btnQueue.hit(mx, my)) {
                            if (authState != AuthState::Authorized) continue;
                            net.send_game({{"type","queue"}});
                            std::lock_guard lk(uiMtx);
                            status = "In queue...";
                            continue;
                        }
                        if (btnResign.hit(mx, my)) {
                            if (authState != AuthState::Authorized) continue;
                            net.send_game({{"type","resign"}});
                            continue;
                        }
                        int fx = mx / TILE;
                        int fy = my / TILE;
                        if (fx < 0 || fx > 7 || fy < 0 || fy > 7) continue;
                        chess::Square sq(chess::File(fx), chess::Rank(7 - fy));

                        if (sel == chess::Square::NO_SQ) {
                            auto p = board.at(sq);
                            if (p != chess::Piece::NONE && myTurn) {
                                bool ownTurn = (iAmWhite && p.color() == chess::Color::WHITE) ||
                                               (!iAmWhite && p.color() == chess::Color::BLACK);
                                if (ownTurn) {
                                    sel = sq;
                                    legalTargets.clear();
                                    chess::Movelist moves;
                                    chess::movegen::legalmoves<chess::movegen::MoveGenType::ALL>(moves, board, -1);
                                    for (auto& m : moves)
                                        if (m.from() == sel) legalTargets.push_back(m.to());
                                }
                            }
                        } else {
                            if (myTurn) {
                                chess::Move mv = chess::Move::make(sel, sq);
                                if (board.isLegal(mv)) {
                                    board.makeMove(mv);
                                    net.send_game({{"type","move"},
                                                   {"from", sq2uci(sel)},
                                                   {"to",   sq2uci(sq)}});
                                    myTurn = false;
                                }
                            }
                            sel = chess::Square::NO_SQ;
                            legalTargets.clear();
                        }
                    }
                }
            }
        }

        if (authState == AuthState::Authorized) {
            drawBoard();
        } else {
            win.clear(sf::Color(30, 30, 40));
            drawForm();
        }
        win.display();
    }
    return 0;
}