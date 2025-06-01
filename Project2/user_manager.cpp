#include "user_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>

UserManager::UserManager(const std::string& filename) : user_file(filename) {}

std::string UserManager::hash_password(const std::string& password) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

bool UserManager::user_exists(const std::string& username) {
    std::ifstream file(user_file);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string stored_username;
        if (std::getline(iss, stored_username, ':')) {
            if (stored_username == username) {
                return true;
            }
        }
    }
    return false;
}

bool UserManager::login(const std::string& username, const std::string& password) {
    spinlock.lock();
    std::ifstream file(user_file);
    if (!file.is_open()) {
        std::cerr << "Failed to open user file for login.\n";
        spinlock.unlock();
        return false;
    }

    std::string line;
    std::string hashed = hash_password(password);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string stored_username, stored_hash;
        if (std::getline(iss, stored_username, ':') && std::getline(iss, stored_hash)) {
            if (stored_username == username && stored_hash == hashed) {
                spinlock.unlock();
                return true;
            }
        }
    }

    spinlock.unlock();
    return false;
}

bool UserManager::register_user(const std::string& username, const std::string& password) {
    spinlock.lock();

    if (user_exists(username)) {
        spinlock.unlock();
        return false;
    }

    std::ofstream file(user_file, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open user file for writing.\n";
        spinlock.unlock();
        return false;
    }

    file << username << ":" << hash_password(password) << "\n";
    file.close();

    spinlock.unlock();
    return true;
}
