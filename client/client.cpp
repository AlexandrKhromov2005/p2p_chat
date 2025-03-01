#include "client.h"
#include <atomic>


AsyncClient::AsyncClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
    : socket_(io_context) {
    tcp::resolver resolver(io_context);
    boost::asio::connect(socket_, resolver.resolve(host, port));
}

void AsyncClient::start() {
    std::thread input_thread([this]() { input_handler(); });
    do_read();
    input_thread.join();
}

void AsyncClient::stop() {
    stopped_ = true;
    socket_.close();
}

void AsyncClient::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, read_buffer_, '\n',
        [this, self](boost::system::error_code ec, size_t length) {
            if (ec || stopped_) {
                if (ec != boost::asio::error::operation_aborted)
                    std::cerr << "Connection closed" << std::endl;
                return;
            }

            std::istream is(&read_buffer_);
            std::string message;
            std::getline(is, message);
            std::cout << "Received: " << message << std::endl;
            
            do_read();
        });
}

void AsyncClient::input_handler() {
    while (!stopped_) {
        std::string message;
        std::getline(std::cin, message);
        
        if (message == "/exit") {
            stop();
            break;
        }
        
        if (!message.empty()) {
            message += "\n";
            boost::asio::write(socket_, boost::asio::buffer(message));
        }
    }
}