#include "game_server.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>

awaitable<std::string> Session::read_line() {
    for (;;) {
        auto pos = readBuf_.find('\n');
        if (pos != std::string::npos) {
            std::string line = readBuf_.substr(0, pos);
            readBuf_.erase(0, pos + 1);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            co_return line;
        }
        char tmp[4096];
        size_t n = co_await sock_.async_read_some(boost::asio::buffer(tmp), use_awaitable);
        if (n == 0) throw boost::system::system_error(boost::asio::error::eof);
        readBuf_.append(tmp, n);
    }
}

void Session::send(json j) {
    std::string line = j.dump() + "\n";
    boost::system::error_code ec;
    boost::asio::write(sock_, boost::asio::buffer(line), ec);
    if (ec) {
        std::cerr << "[srv] send error: " << ec.message() << "\n";
        close();
    }
}

awaitable<void> Session::run() {
    try {
        for (;;) {
            std::string line = co_await read_line();
            if (line.empty()) continue;
            json j = json::parse(line, nullptr, false);
            if (j.is_discarded()) continue;

            auto t = j.value("type", "");
            std::cerr << "[srv] got type=" << t
                      << " login=" << (login_.empty() ? "?" : login_) << "\n";

            if (t == "register") {
                std::string login = j["login"], pass = j["pass"];
                std::string h = std::to_string(std::hash<std::string>{}(pass));
                if (srv_.db.register_user(login, h)) {
                    send({{"type","ok"},{"rating",1200}});
                } else {
                    send({{"type","error"},{"msg","login exists"}});
                }
            }
            else if (t == "login") {
                std::string login = j["login"];
                std::string h = std::to_string(std::hash<std::string>{}(std::string(j["pass"])));
                auto u = srv_.db.get_user(login);
                if (!u || u->pass_hash != h) {
                    send({{"type","error"},{"msg","bad credentials"}});
                    continue;
                }

                // Проверка: не залогинен ли уже этот логин
                std::shared_ptr<Session> existing;
                {
                    std::lock_guard lk(srv_.mtx);
                    auto it = srv_.online.find(login);
                    if (it != srv_.online.end()) existing = it->second;
                }

                if (existing && existing != shared_from_this()) {
                    // Убиваем старую сессию
                    std::cerr << "[srv] login: killing previous session for " << login << "\n";
                    existing->send({{"type","error"},{"msg","logged in from another client"}});
                    existing->close();
                    {
                        std::lock_guard lk(srv_.mtx);
                        srv_.online.erase(login);
                    }
                }

                login_ = login;
                rating_ = u->rating;
                {
                    std::lock_guard lk(srv_.mtx);
                    srv_.online[login_] = shared_from_this();
                }
                send({{"type","ok"},{"rating",rating_}});
            }
            else if (t == "queue") {
                if (login_.empty()) { send({{"type","error"},{"msg","auth first"}}); continue; }
                srv_.enqueue(shared_from_this());
            }
            else if (t == "move") {
                srv_.handle_move(shared_from_this(), j);
            }
            else if (t == "resign") {
                srv_.handle_resign(shared_from_this());
            }
        }
    } catch (...) {}

    co_await boost::asio::steady_timer(
        co_await boost::asio::this_coro::executor,
        std::chrono::milliseconds(100)).async_wait(use_awaitable);

    srv_.logout(shared_from_this());
}

awaitable<void> GameServer::listen(unsigned short port) {
    tcp::acceptor a(io_, tcp::endpoint(tcp::v4(), port));
    std::cout << "[game] listening on " << port << std::endl;
    for (;;) {
        tcp::socket s = co_await a.async_accept(use_awaitable);
        auto ses = std::make_shared<Session>(std::move(s), *this);
        co_spawn(io_, [ses]() -> awaitable<void> {
            co_await ses->run();
        }, detached);
    }
}

void GameServer::enqueue(std::shared_ptr<Session> s) {
    std::lock_guard lk(mtx);
    if (std::find(queue.begin(), queue.end(), s) != queue.end()) return;
    queue.push_back(s);
    for (size_t i = 0; i + 1 < queue.size(); ) {
        auto a = queue[i], b = queue[i+1];
        if (std::abs(a->rating_ - b->rating_) <= 300) {
            queue.erase(queue.begin() + i, queue.begin() + i + 2);
            start_match(a, b);
        } else ++i;
    }
}

void GameServer::start_match(std::shared_ptr<Session> a, std::shared_ptr<Session> b) {
    a->in_game_ = b->in_game_ = true;
    a->is_white_ = true;  b->is_white_ = false;
    a->opponent_ = b;     b->opponent_ = a;
    a->send({{"type","matched"},{"white",true},{"opp",b->login_},{"rating",b->rating_}});
    b->send({{"type","matched"},{"white",false},{"opp",a->login_},{"rating",a->rating_}});
}

void GameServer::handle_move(std::shared_ptr<Session> s, const json& j) {
    auto opp = s->opponent_.lock();
    if (!opp) return;
    opp->send({{"type","move"},{"from",j["from"]},{"to",j["to"]}});
}

void GameServer::handle_resign(std::shared_ptr<Session> s) {
    auto opp = s->opponent_.lock();
    if (!opp) return;
    end_game(s, opp, /*white_won=*/ !s->is_white_);
}

void GameServer::end_game(std::shared_ptr<Session> loser, std::shared_ptr<Session> winner,
                          bool white_won) {
    int rw = winner->rating_, rl = loser->rating_;
    double Ew = 1.0 / (1.0 + std::pow(10.0, (rl - rw) / 400.0));
    int new_w = rw + int(std::round(32 * (1.0 - Ew)));
    int new_l = rl + int(std::round(32 * (0.0 - (1.0 - Ew))));
    winner->rating_ = new_w;
    loser->rating_  = new_l;
    db.update_rating(winner->login_, new_w);
    db.update_rating(loser->login_, new_l);
    db.save_game(winner->is_white_ ? winner->login_ : loser->login_,
                 winner->is_white_ ? loser->login_ : winner->login_,
                 white_won ? "white" : "black");

    winner->send({{"type","game_over"},{"result","win"},{"rating",new_w}});
    loser->send({{"type","game_over"},{"result","loss"},{"rating",new_l}});
    winner->in_game_ = loser->in_game_ = false;
    winner->opponent_.reset();
    loser->opponent_.reset();
}

void GameServer::logout(std::shared_ptr<Session> s) {
    std::lock_guard lk(mtx);
    // удаляем только если это та же сессия (не убитая предыдущим login)
    auto it = online.find(s->login_);
    if (it != online.end() && it->second == s) online.erase(it);

    queue.erase(std::remove(queue.begin(), queue.end(), s), queue.end());

    if (s->in_game_) {
        auto opp = s->opponent_.lock();
        s->in_game_ = false;
        s->opponent_.reset();
        if (opp) {
            opp->in_game_ = false;
            opp->opponent_.reset();
            opp->send({{"type","game_over"},{"result","win"},{"rating",opp->rating_}});
        }
    }
}