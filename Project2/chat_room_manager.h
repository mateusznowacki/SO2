#ifndef CHAT_ROOM_MANAGER_H
#define CHAT_ROOM_MANAGER_H

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

#endif
