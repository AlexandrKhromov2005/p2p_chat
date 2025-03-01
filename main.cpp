#include "server/server.h"
#include "client/client.h"
#include <memory>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: p2p_chat <server|client>" << std::endl;
        return 1;
    }

    boost::asio::io_context io_context;

    if (std::strcmp(argv[1], "server") == 0) {
        AsyncServer server(io_context, 12345);
        std::thread io_thread([&io_context](){ io_context.run(); });
        
        // Обработка ввода для сервера
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == "/exit") break;
            // Здесь можно добавить логику рассылки сообщений
        }
        
        io_context.stop();
        io_thread.join();
    } 
    else if (std::strcmp(argv[1], "client") == 0) {
        auto client = std::make_shared<AsyncClient>(io_context, "127.0.0.1", "12345");
        client->start();
        io_context.run();
    }
    else {
        std::cerr << "Invalid argument! Use 'server' or 'client'." << std::endl;
        return 1;
    }

    return 0;
}