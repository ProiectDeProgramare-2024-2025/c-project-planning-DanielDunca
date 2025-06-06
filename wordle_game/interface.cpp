#include <iostream>
#include "interface.h"
#include "../shared/colors.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <vector>
#include <algorithm>
#ifdef _WIN32
struct ColorInitializer {
    ColorInitializer() { enableANSIColors(); }
} colorInitializer;
#endif
void menu() {
    std::cout << "-=-=-=-=-=-" IBLUE "CLI " RESET   GREEN "W" RESET RED "or" RESET  YELLOW "d" RESET  GREEN "le" RESET "-=-=-=-=-" << std::endl;
    std::cout << "1. " CYAN "Play" RESET        << std::endl;
    std::cout << "2. " CYAN "About" RESET       << std::endl;
    std::cout << "3. " CYAN "Stats" RESET       << std::endl;
    std::cout << "4. " CYAN "Leaderboard" RESET << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "0. " RED "Exit" RESET << std::endl;
}
void about() {
    std::cout << BOLD "=========== Wordle Rules ===========" RESET << std::endl;
    std::cout << CYAN "1." RESET " Guess the hidden word in 6 tries." << std::endl;
    std::cout << CYAN "2." RESET " Each guess must be a valid word." << std::endl;
    std::cout << CYAN "3." RESET " After each guess, the letters of your word" << std::endl;
    std::cout << " will be marked as follows:" << std::endl;
    std::cout << "   - " GREEN "Green" RESET ": Correct letter in the correct spot." << std::endl;
    std::cout << "   - " YELLOW "Yellow" RESET ": Correct letter but wrong spot." << std::endl;
    std::cout << "   - " GRAY "Grey" RESET ": Letter is not in the word." << std::endl;
    std::cout << BOLD "=====================================" RESET << std::endl;
}
void play() {
    std::cout << BOLD "-=-=-       Choose Difficulty       -=-=-" RESET << std::endl;
    std::cout << BOLD "=========================================" RESET << std::endl;
    std::cout << "1. " GREEN "Easy" RESET << std::endl;
    std::cout << "2. " YELLOW "Medium" RESET << std::endl;
    std::cout << "3. " RED "Hard" RESET << std::endl;
    std::cout << BOLD "=========================================" RESET << std::endl;

}
void stats() {
    std::cout << BOLD "=========== Player Stats ===========" RESET << std::endl;

    std::string current_user;
    {
        std::ifstream session("shared/session.txt");
        if (session.is_open()) {
            std::getline(session, current_user);
        }
    }

    std::ifstream stats_file("shared/stats.csv");
    bool printed = false;
    if (stats_file.is_open()) {
        std::string line;
        while (std::getline(stats_file, line)) {
            std::stringstream ss(line);
            std::string user, wEasy, wMed, wHard, losses, tries;
            std::getline(ss, user, ',');
            std::getline(ss, wEasy, ',');
            std::getline(ss, wMed, ',');
            std::getline(ss, wHard, ',');
            std::getline(ss, losses, ',');
            std::getline(ss, tries);
            if (user == current_user) {
                std::cout << CYAN "User:" RESET " " << user << std::endl;
                std::cout << CYAN "Wins Easy:" RESET " " << wEasy << std::endl;
                std::cout << CYAN "Wins Medium:" RESET " " << wMed << std::endl;
                std::cout << CYAN "Wins Hard:" RESET " " << wHard << std::endl;
                std::cout << CYAN "Losses:" RESET " " << losses << std::endl;
                std::cout << CYAN "Tries used:" RESET " " << tries << std::endl;
                printed = true;
                break;
            }
        }
    }

    if (!printed) {
        if (!current_user.empty()) {
            std::cout << RED "No stats available for " RESET << current_user << "." << std::endl;
        } else {
            std::cout << RED "No user logged in." RESET << std::endl;
        }
    }

    std::cout << BOLD "=====================================" RESET << std::endl;
}

void leaderboard() {
    std::cout << BOLD "======================== Leaderboard ========================" RESET << std::endl;
    std::ifstream stats_file("shared/stats.csv");
    struct Entry {
        std::string user;
        int easy{};
        int med{};
        int hard{};
        int losses{};
        int tries{};
    };
    std::vector<Entry> entries;
    if (stats_file.is_open()) {
        std::string line;
        std::getline(stats_file, line);
        while (std::getline(stats_file, line)) {
            std::stringstream ss(line);
            Entry e;
            std::string easy, med, hard, losses, tries;
            std::getline(ss, e.user, ',');
            std::getline(ss, easy, ',');
            std::getline(ss, med, ',');
            std::getline(ss, hard, ',');
            std::getline(ss, losses, ',');
            std::getline(ss, tries);
            e.easy   = std::stoi(easy);
            e.med    = std::stoi(med);
            e.hard   = std::stoi(hard);
            e.losses = std::stoi(losses);
            e.tries  = std::stoi(tries);
            entries.push_back(e);
        }
    }
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        int totalA = a.easy + a.med + a.hard;
        int totalB = b.easy + b.med + b.hard;
        return totalA > totalB;
    });

    for (const auto& e : entries) {
        std::cout << CYAN << e.user << RESET
                  << " | Easy: " << e.easy
                  << " | Medium: " << e.med
                  << " | Hard: " << e.hard
                  << " | Losses: " << e.losses
                  << " | Tries: " << e.tries
                  << std::endl;
    }
    std::cout << BOLD "==============================================================" RESET << std::endl;
}
void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


int promptForChoice(const std::string& prompt, const std::vector<int>& validOptions) {
    int value;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. ";
            continue;
        }
        if (std::find(validOptions.begin(), validOptions.end(), value) != validOptions.end()) {
            return value;
        }
        std::cout << "Invalid choice. ";
    }
}