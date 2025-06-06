#include "login_manager.h"
#include <fstream>
#include <filesystem>
#include <sstream>

LoginManager::LoginManager(const std::string& loginFile,
                           const std::string& sessionFile,
                           const std::string& statsFile)
    : loginFilePath_(loginFile),
      sessionFilePath_(sessionFile),
      statsFilePath_(statsFile) {
    if (!std::filesystem::exists(loginFilePath_)) {
        std::ofstream header(loginFilePath_);
        header << "Username,Password\n";
    }
    if (!std::filesystem::exists(statsFilePath_)) {
        std::ofstream header(statsFilePath_);
        header << "username,wins_easy,wins_medium,wins_hard,losses,tries\n";
    }
}

bool LoginManager::usernameExists(const std::string& username) const {
    std::ifstream file(loginFilePath_);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fileUser;
        std::getline(ss, fileUser, ',');
        if (fileUser == username) {
            return true;
        }
    }
    return false;
}

bool LoginManager::isCorrectCredentials(const std::string& username,
                                        const std::string& password) const {
    std::ifstream file(loginFilePath_);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fileUser, filePass;
        std::getline(ss, fileUser, ',');
        std::getline(ss, filePass);
        if (fileUser == username && filePass == password) {
            return true;
        }
    }
    return false;
}

bool LoginManager::signUp(const std::string& username, const std::string& password) {
    std::ofstream loginFile(loginFilePath_, std::ios::app);
    if (!loginFile) return false;
    loginFile << username << ',' << password << "\n";

    std::ofstream statsFile(statsFilePath_, std::ios::app);
    if (statsFile) {
        statsFile << username << ",0,0,0,0,0\n";
    }
    return true;
}

bool LoginManager::login(const std::string& username, const std::string& password) {
    if (isCorrectCredentials(username, password)) {
        std::ofstream session(sessionFilePath_);
        session << username << '\n';
        return true;
    }
    return false;
}

void LoginManager::logout() {
    if (std::filesystem::exists(sessionFilePath_)) {
        std::filesystem::remove(sessionFilePath_);
    }
}