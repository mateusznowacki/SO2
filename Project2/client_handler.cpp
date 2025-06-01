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
    // Usuwamy wszelkie pojedyncze \n i \r na końcu
    while (!formatted.empty() && (formatted.back() == '\n' || formatted.back() == '\r')) {
        formatted.pop_back();
    }

    formatted += "\r\n"; // zawsze na końcu dokładnie \r\n

#ifdef _WIN32
    send(client_fd, formatted.c_str(), static_cast<int>(formatted.length()), 0);
#else
    write(client_fd, formatted.c_str(), formatted.length());
#endif
}


std::string ClientHandler::receive_message() {
    char buffer[BUFFER_SIZE] = {0};
    std::string result;

    do {
#ifdef _WIN32
        int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
#else
        int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);
#endif
        if (bytes <= 0) {
            std::cerr << "[DEBUG] Client disconnected or read error.\n";
            return "";
        }

        result = std::string(buffer, bytes);

        // czyścimy CR i LF
        result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
        result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

        // dopóki odbieramy same puste linie — kontynuujemy czytanie
    } while (result.empty());

    return result;
}


bool ClientHandler::authenticate_user(std::string& username) {
    auto strip = [](std::string& s) {
        s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
        s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    };

    send_message("Do you have an account? (yes/no): ");
    std::string answer;
    do {
        answer = receive_message();
        strip(answer);
    } while (answer.empty());

    send_message("Enter username: ");
    do {
        username = receive_message();
        strip(username);
    } while (username.empty());

    send_message("Enter password: ");
    std::string password;
    do {
        password = receive_message();
        strip(password);
    } while (password.empty());

    if (answer == "yes") {
        if (user_manager->login(username, password)) {
            send_message("Login successful.\r\nEnter chat room name: ");
            return true;
        } else {
            send_message("Invalid credentials.\n");
            return false;
        }
    } else if (answer == "no") {
        if (user_manager->register_user(username, password)) {
            send_message("Account created. You are now logged in.\r\nEnter chat room name: ");
            return true;
        } else {
            send_message("Username already exists.\n");
            return false;
        }
    } else {
        send_message("Invalid option. Please restart the client.\n");
        return false;
    }
}

void ClientHandler::handle() {
    std::string username;
    auto strip = [](std::string& s) {
        s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
        s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    };
    if (!authenticate_user(username)) {
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return;
    }

    std::string room_name;
    do {
        room_name = receive_message();

        strip(room_name);
    } while (room_name.empty());

    ChatRoom* room = room_manager->get_or_create_room(room_name);
    room->add_client(client_fd);
    room->send_history(client_fd);

    send_message("You have joined room: " + room_name + "\n");

    while (true) {
        std::string msg = receive_message();
        if (msg.empty()) break;

        strip(msg);
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
