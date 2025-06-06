#ifndef LOGIN_MANAGER_H
#define LOGIN_MANAGER_H

#include <string>

class LoginManager {
public:
    LoginManager(const std::string& loginFile,
                 const std::string& sessionFile,
                 const std::string& statsFile);

    bool signUp(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool usernameExists(const std::string& username) const;

private:
    bool isCorrectCredentials(const std::string& username,
                              const std::string& password) const;
    std::string loginFilePath_;
    std::string sessionFilePath_;
    std::string statsFilePath_;
};

#endif // LOGIN_MANAGER_H