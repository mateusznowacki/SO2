#include "client_handler.h"
#include <iostream>
#include <cstring>
#include <algorithm> // dla std::remove

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#endif

#define BUFFER_SIZE 1024

ClientHandler::ClientHandler(int client_socket, UserManager* userMgr, ChatRoomManager* roomMgr)
    : client_fd(client_socket), user_manager(userMgr), room_manager(roomMgr) {}

void ClientHandler::send_message(const std::string& msg) {
    std::string formatted = msg;
    if (!msg.empty() && msg.back() != '\n') {
        formatted += "\r\n"; // lepiej niż tylko \n
    }
#ifdef _WIN32
    send(client_fd, formatted.c_str(), static_cast<int>(formatted.length()), 0);
#else
    write(client_fd, formatted.c_str(), formatted.length());
#endif
}


std::string ClientHandler::receive_message() {
    char buffer[BUFFER_SIZE] = {0};
#ifdef _WIN32
    int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
#else
    int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);
#endif
    if (bytes <= 0) {
        std::cerr << "[DEBUG] Client disconnected or read error.\n";
        return "";
    }
    return std::string(buffer, bytes);
}


bool ClientHandler::authenticate_user(std::string& username) {
    auto strip = [](std::string& s) {
        s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
        s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    };

    send_message("Do you have an account? (yes/no): ");
    std::string answer = receive_message();
    do {
        answer = receive_message();
    } while (answer.empty());

    send_message("Enter username: ");
    username = receive_message();
    do {
        answer = receive_message();
    } while (answer.empty());

    send_message("Enter password: ");
    std::string password = receive_message();
    do {
    answer = receive_message();
} while (answer.empty());

    strip(username);
    strip(password);
    strip(answer);

    if (answer == "yes") {
        if (user_manager->login(username, password)) {
            send_message("Login successful.\n");
            return true;
        } else {
            send_message("Invalid credentials.\n");
            return false;
        }
    } else {
        if (user_manager->register_user(username, password)) {
            send_message("Account created. You are now logged in.\n");
            return true;
        } else {
            send_message("Username already exists.\n");
            return false;
        }
    }
}

void ClientHandler::handle() {
    std::string username;
    if (!authenticate_user(username)) {
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return;
    }

    send_message("Enter chat room name: ");
    std::string room_name = receive_message();
    room_name.erase(std::remove(room_name.begin(), room_name.end(), '\r'), room_name.end());
    room_name.erase(std::remove(room_name.begin(), room_name.end(), '\n'), room_name.end());

    ChatRoom* room = room_manager->get_or_create_room(room_name);
    room->add_client(client_fd);
    room->send_history(client_fd);

    send_message("You have joined room: " + room_name + "\n");

    while (true) {
        std::string msg = receive_message();
        if (msg.empty()) break;

        msg.erase(std::remove(msg.begin(), msg.end(), '\r'), msg.end());
        msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());

        if (msg.empty()) continue;

        std::string full_msg = username + ": " + msg + "\n";
        room->broadcast_message(full_msg, client_fd);
    }

    room->remove_client(client_fd);

#ifdef _WIN32
    closesocket(client_fd);
#else
    close(client_fd);
#endif
}
