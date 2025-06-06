#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <sstream>
#include "login_manager.h"
#include "../shared/colors.h"
void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
int main(int argc, char* argv[]) {
    clear_console();
#ifdef _WIN32
    enableANSIColors();
#endif
    clear_console();
    int a;
    std::string name;
    std::string password;

    std::string loginFilePath = "shared/login.csv";
    std::string sessionFilePath = "shared/session.txt";
    std::string statsFilePath = "shared/stats.csv";
    if (argc > 1) loginFilePath = argv[1];
    if (argc > 2) sessionFilePath = argv[2];
    if (argc > 3) statsFilePath  = argv[3];

    LoginManager manager(loginFilePath, sessionFilePath, statsFilePath);
    std::cout << UNDERLINE "Welcome" RESET " to "  GREEN "W" RESET RED "or" RESET  YELLOW "d" RESET  GREEN"le " RESET "Login!" << std::endl;
    std::cout << "========================"<< std::endl;
    std::cout << "1. " CYAN "Sign Up" RESET << std::endl;
    std::cout << "2. " CYAN "Login" RESET  << std::endl;
    std::cout << "3. " CYAN "Log Out" RESET << std::endl;
    std::cout << "========================"<< std::endl;
    std::cin >> a;


    if (a == 1) {
        clear_console();
        std::cout << "Enter your " UNDERLINE "username" RESET ": ";
        std::cin >> name;
        std::cout << "Enter your " UNDERLINE "password" RESET ": ";
        std::cin >> password;
        manager.signUp(name, password);

        std::cout << GREEN "Thank " RESET "you for " UNDERLINE "signing up" RESET "."  << std::endl;

    }

    if (a == 2) {
        clear_console();
        std::cout << "Enter your " UNDERLINE "username" RESET ": ";
        std::cin >> name;
        std::cout << "Enter your " UNDERLINE "password" RESET ": ";
        std::cin >> password;

        if (manager.login(name, password)) {
            std::cout << GREEN "Thank you" RESET " for logging in. Session started." << std::endl;
        } else if (manager.usernameExists(name)) {
            std::cout << RED "Incorrect password" RESET ". Please try again." << std::endl;
        } else {
            std::cout << RED "Account not found." RESET " Please " BOLD "sign up" RESET " first." << std::endl;
        }
    }


    if (a == 3) {
        clear_console();
        bool wasLoggedIn = std::filesystem::exists(sessionFilePath);
        manager.logout();
        if (wasLoggedIn) {
            std::cout << GREEN "User logged out." RESET " Session file deleted." << std::endl;
        } else {
            std::cout << RED "No user is currently logged in." RESET << std::endl;
        }
    }

    return 0;
}
