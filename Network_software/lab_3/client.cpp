#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0]
                  << " <IP-адрес> <порт>" << std::endl;
        return 1;
    }

    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Подключение установлено. Введите сообщение "
                     "(Ctrl+D — выход):" << std::endl;

        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) continue;

            boost::asio::write(socket, boost::asio::buffer(line));

            char reply[1024];
            std::size_t reply_length =
                socket.read_some(boost::asio::buffer(reply));
            std::cout << "Ответ сервера: "
                      << std::string(reply, reply_length) << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return 0;
}