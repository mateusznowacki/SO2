#ifndef PROJECT2_HEADERS_CHAT_ROOM_MANAGER_H_
#define PROJECT2_HEADERS_CHAT_ROOM_MANAGER_H_

#include <map>
#include <string>
#include "chat_room.h"
#include "SpinLock.h"

class ChatRoomManager {
 private:
    std::map<std::string, ChatRoom*> rooms;
    SpinLock manager_mutex;

 public:
    ChatRoomManager();
    ~ChatRoomManager();

    ChatRoom* get_or_create_room(const std::string& room_name);
};

#endif  // PROJECT2_HEADERS_CHAT_ROOM_MANAGER_H_
