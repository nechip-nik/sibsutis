#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include "protocol.hpp"

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;

class ChatServer {
public:
    explicit ChatServer(boost::asio::io_context& io) : io_(io) {}
    awaitable<void> listen(unsigned short port);
    awaitable<void> session(std::shared_ptr<tcp::socket> sock);
private:
    boost::asio::io_context& io_;
    // login -> socket
    std::unordered_map<std::string, std::shared_ptr<tcp::socket>> clients_;
    std::mutex mtx_;
};