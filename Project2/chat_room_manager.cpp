#include "chat_room_manager.h"

ChatRoomManager::ChatRoomManager() {}

ChatRoomManager::~ChatRoomManager() {
    for (auto& pair : rooms) {
        delete pair.second;
    }
}

ChatRoom* ChatRoomManager::get_or_create_room(const std::string& room_name) {
    manager_mutex.lock();

    if (rooms.find(room_name) == rooms.end()) {
        rooms[room_name] = new ChatRoom(room_name);
    }

    ChatRoom* room = rooms[room_name];

    manager_mutex.unlock();
    return room;
}
