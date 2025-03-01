#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <thread>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void start();

private:
    void do_read();
    void do_write(const std::string& message);

    tcp::socket socket_;
    boost::asio::streambuf read_buffer_;
};

class AsyncServer {
public:
    AsyncServer(boost::asio::io_context& io_context, short port);
    
private:
    void do_accept();

    tcp::acceptor acceptor_;
};

#endif // SERVER_H
