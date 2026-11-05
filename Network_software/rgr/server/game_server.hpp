#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <cmath>
#include <optional>
#include <string>
#include "db.hpp"
#include "protocol.hpp"

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;

class Session;
class GameServer;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket s, GameServer& srv)
        : sock_(std::move(s)), srv_(srv) {}

    awaitable<void> run();
    void send(json j);
    awaitable<std::string> read_line();
    void close() { boost::system::error_code ec; sock_.close(ec); }

    std::string login_;
    int rating_ = 1200;
    bool in_game_ = false;
    bool is_white_ = false;
    std::weak_ptr<Session> opponent_;

private:
    tcp::socket sock_;
    GameServer& srv_;
    std::string readBuf_;
};

class GameServer {
public:
    explicit GameServer(boost::asio::io_context& io) : io_(io) {}

    DB db;
    std::mutex mtx;
    std::unordered_map<std::string, std::shared_ptr<Session>> online;
    std::vector<std::shared_ptr<Session>> queue;

    awaitable<void> listen(unsigned short port);
    void enqueue(std::shared_ptr<Session> s);
    void start_match(std::shared_ptr<Session> a, std::shared_ptr<Session> b);
    void handle_move(std::shared_ptr<Session> s, const json& j);
    void handle_resign(std::shared_ptr<Session> s);
    void end_game(std::shared_ptr<Session> loser, std::shared_ptr<Session> winner,
                  bool white_won);
    void logout(std::shared_ptr<Session> s);

private:
    boost::asio::io_context& io_;
};