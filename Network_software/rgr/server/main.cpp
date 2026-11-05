#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include "game_server.hpp"
#include "chat_server.hpp"

int main() {
    try {
        boost::asio::io_context io;

        GameServer gs(io);
        gs.db.open("chess.db");
        gs.db.init();

        ChatServer cs(io);

        co_spawn(io, gs.listen(5555), detached);
        co_spawn(io, cs.listen(5556), detached);

        unsigned n = std::max(2u, std::thread::hardware_concurrency());
        std::vector<std::thread> pool;
        for (unsigned i = 0; i < n; ++i)
            pool.emplace_back([&]{ io.run(); });
        for (auto& t : pool) t.join();
    } catch (std::exception& e) {
        std::cerr << "fatal: " << e.what() << "\n";
        return 1;
    }
}