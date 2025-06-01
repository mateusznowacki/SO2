#ifndef PROJECT2_HEADERS_CLIENT_HANDLER_H_
#define PROJECT2_HEADERS_CLIENT_HANDLER_H_

#include <string>
#include "user_manager.h"
#include "chat_room_manager.h"

class ClientHandler {
  private:
    int client_fd;
    UserManager* user_manager;
    ChatRoomManager* room_manager;

    bool authenticate_user(std::string& username);
    void send_message(const std::string& msg);
    std::string receive_message();

  public:
    ClientHandler(int client_socket, UserManager* userMgr, ChatRoomManager* roomMgr);
    void handle();
};

#endif  //  PROJECT2_HEADERS_CLIENT_HANDLER_H
