#include "server.h"

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() {
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, read_buffer_, '\n',
        [this, self](boost::system::error_code ec, size_t length) {
            if (ec) {
                if (ec != boost::asio::error::eof)
                    std::cerr << "Error: " << ec.message() << std::endl;
                return;
            }

            std::istream is(&read_buffer_);
            std::string message;
            std::getline(is, message);
            std::cout << "Received: " << message << std::endl;
            
            do_read();
        });
}

AsyncServer::AsyncServer(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    do_accept();
}

void AsyncServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
                std::cout << "New connection!" << std::endl;
            }
            do_accept();
        });
}