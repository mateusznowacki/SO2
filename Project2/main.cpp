#include "user_manager.h"
#include "client_handler.h"
#include "chat_room_manager.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <thread>




#define PORT 23
#define MAX_CONNECTIONS 10

UserManager user_manager("data/users.txt");
ChatRoomManager room_manager;

void client_thread(int client_fd) {
    ClientHandler handler(client_fd, &user_manager, &room_manager);
    handler.handle();
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        return 1;
    }

    if (listen(server_fd, MAX_CONNECTIONS) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        return 1;
    }

    std::cout << "Server is running on port " << PORT << "...\n";

    while (true) {
        SOCKET client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            continue;
        }

        // Wyłączamy Nagle Algorithm (wymuszamy natychmiastowe send() bez buforowania TCP)
        int flag = 1;
        setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char *>(&flag), sizeof(flag));

        std::thread(client_thread, client_fd).detach();
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
