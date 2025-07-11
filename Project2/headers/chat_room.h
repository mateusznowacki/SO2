#ifndef PROJECT2_HEADERS_CHAT_ROOM_H_
#define PROJECT2_HEADERS_CHAT_ROOM_H_

#include <string>
#include <set>
#include "SpinLock.h"

class ChatRoom {
 private:
    std::string room_name;
    std::string history_filename;
    std::set<int> clients;
    SpinLock room_mutex;

 public:
    explicit ChatRoom(const std::string& name);

    void add_client(int client_fd);
    void remove_client(int client_fd);
    void broadcast_message(const std::string& message, int sender_fd);
    void save_message(const std::string& message);
    void send_history(int client_fd);
};

#endif  //PROJECT2_HEADERS_CHAT_ROOM_H_
