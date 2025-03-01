#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <memory>  // Добавлен заголовок

using boost::asio::ip::tcp;

class AsyncClient : public std::enable_shared_from_this<AsyncClient> {  
public:
    AsyncClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port);
    void start();
    void stop();

private:
    void do_read();
    void input_handler();

    tcp::socket socket_;
    boost::asio::streambuf read_buffer_;
    std::atomic<bool> stopped_{false};
};

#endif // CLIENT_H