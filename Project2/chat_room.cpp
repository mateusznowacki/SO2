#include "chat_room.h"
#include <fstream>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif

ChatRoom::ChatRoom(const std::string& name) : room_name(name) {
    history_filename = "data/history_" + room_name + ".txt";
}

void ChatRoom::add_client(int client_fd) {
    room_mutex.lock();
    clients.insert(client_fd);
    room_mutex.unlock();
}

void ChatRoom::remove_client(int client_fd) {
    room_mutex.lock();
    clients.erase(client_fd);
    room_mutex.unlock();
}

void ChatRoom::broadcast_message(const std::string& message, int sender_fd) {
    std::string formatted = message;
    if (formatted.back() != '\n') {
        formatted += "\r\n";
    } else if (formatted.length() >= 2 && formatted.substr(formatted.length() - 2) != "\r\n") {
        formatted.insert(formatted.length() - 1, "\r");
    }

    room_mutex.lock();
    for (int fd : clients) {
        if (fd != sender_fd) {
#ifdef _WIN32
            send(fd, formatted.c_str(), static_cast<int>(formatted.length()), 0);
#else
            write(fd, formatted.c_str(), formatted.length());
#endif
        }
    }
    save_message(formatted); // zapisujemy sformatowaną wersję
    room_mutex.unlock();
}

void ChatRoom::save_message(const std::string& message) {
    std::ofstream file(history_filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << history_filename << " for writing.\n";
        return;
    }

    file << message;
    file.close();
}


void ChatRoom::send_history(int client_fd) {
    std::ifstream file(history_filename);
    std::string line;
    while (std::getline(file, line)) {
        std::string msg = line + "\n";
#ifdef _WIN32
        send(client_fd, msg.c_str(), static_cast<int>(msg.size()), 0);
#else
        write(client_fd, msg.c_str(), msg.size());
#endif
    }
}
