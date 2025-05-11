#pragma once

#include <string>
#include "SpinLock.h"

class UserManager {
private:
    std::string user_file;
    SpinLock spinlock;

    bool user_exists(const std::string& username);
    std::string hash_password(const std::string& password);

public:
    UserManager(const std::string& filename);
    bool login(const std::string& username, const std::string& password);
    bool register_user(const std::string& username, const std::string& password);
};
