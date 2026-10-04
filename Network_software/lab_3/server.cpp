#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

using boost::asio::ip::tcp;


std::string reverse_words(const std::string& input) {
    std::string result;
    std::string word;
    for (char c : input) {
        if (c == ' ') {
            if (!word.empty()) {
                std::reverse(word.begin(), word.end());
                result += word + " ";
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        std::reverse(word.begin(), word.end());
        result += word;
    }

    if (!result.empty() && result.back() == ' ')
        result.pop_back();
    return result;
}

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string msg(data_, length);
                    std::cout << "Получено: " << msg << std::endl;
                    std::string reversed = reverse_words(msg);
                    std::cout << "Ответ: " << reversed << std::endl;
                    do_write(reversed);
                }
            });
    }

    void do_write(const std::string& msg) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(msg),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read(); 
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
public:
    Server(boost::asio::io_context& io_context)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), 0)) {

        unsigned short port = acceptor_.local_endpoint().port();
        std::cout << "Сервер запущен. Порт: " << port << std::endl;
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept(); 
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
    }
    return 0;
}