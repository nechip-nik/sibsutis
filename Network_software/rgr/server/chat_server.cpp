#include "chat_server.hpp"

awaitable<void> ChatServer::listen(unsigned short port) {
    tcp::acceptor acc(io_, tcp::endpoint(tcp::v4(), port));
    std::cout << "[chat] listening on " << port << std::endl;
    for (;;) {
        auto sock = std::make_shared<tcp::socket>(io_);
        co_await acc.async_accept(*sock, use_awaitable);
        co_spawn(io_, [this, sock]() -> awaitable<void> {
            co_await session(sock);
        }, detached);
    }
}

awaitable<void> ChatServer::session(std::shared_ptr<tcp::socket> sock) {
    std::string readBuf;
    std::string myLogin;
    try {
        for (;;) {
            std::string line;
            for (;;) {
                auto pos = readBuf.find('\n');
                if (pos != std::string::npos) {
                    line = readBuf.substr(0, pos);
                    readBuf.erase(0, pos + 1);
                    if (!line.empty() && line.back() == '\r') line.pop_back();
                    break;
                }
                char tmp[4096];
                size_t n = co_await sock->async_read_some(boost::asio::buffer(tmp), use_awaitable);
                if (n == 0) throw boost::system::system_error(boost::asio::error::eof);
                readBuf.append(tmp, n);
            }
            auto j = json::parse(line, nullptr, false);
            if (j.is_discarded()) continue;

            auto t = j.value("type", "");
            if (t == "hello") {
                // клиент представляется: {"type":"hello","login":"alice"}
                std::string login = j.value("login", "");
                if (login.empty()) continue;
                myLogin = login;
                {
                    std::lock_guard lk(mtx_);
                    clients_[myLogin] = sock;
                }
                std::cerr << "[chat] hello from " << myLogin << "\n";
            }
            else if (t == "chat") {
                // {"type":"chat","from":"alice","to":"bob","text":"hi"}
                std::string from = j.value("from", myLogin);
                std::string to   = j.value("to", "");
                std::string text = j.value("text", "");
                json out = {{"type","chat"},{"from",from},{"to",to},{"text",text}};
                std::string payload = out.dump() + "\n";

                std::lock_guard lk(mtx_);
                // отправить обоим участникам
                for (auto& target : { from, to }) {
                    if (target.empty()) continue;
                    auto it = clients_.find(target);
                    if (it == clients_.end()) continue;
                    auto s = it->second;
                    boost::system::error_code ec;
                    boost::asio::write(*s, boost::asio::buffer(payload), ec);
                }
            }
        }
    } catch (...) {
        std::lock_guard lk(mtx_);
        if (!myLogin.empty()) clients_.erase(myLogin);
    }
}