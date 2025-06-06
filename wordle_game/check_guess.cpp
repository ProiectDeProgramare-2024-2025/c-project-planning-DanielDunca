#include "check_guess.h"
#include "interface.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <cctype>
#include <iostream>
#include <algorithm>

#include "../shared/colors.h"
constexpr char const* CLEAR_SCREEN = "\033[2J\033[H";
void WordleGame::update_stats(bool won, int tries_used, const std::string& difficulty) {
    std::string current_user;
    {
        std::ifstream session("shared/session.txt");
        if (session.is_open()) {
            std::getline(session, current_user);
        }
    }
    if (current_user.empty()) {
        return;
    }

    std::ifstream in("shared/stats.csv");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    in.close();

    for (std::string& l : lines) {
        std::stringstream ss(l);
        std::string user, wEasy, wMed, wHard, losses, tries;
        std::getline(ss, user, ',');
        std::getline(ss, wEasy, ',');
        std::getline(ss, wMed, ',');
        std::getline(ss, wHard, ',');
        std::getline(ss, losses, ',');
        std::getline(ss, tries);

        if (user == "username") {
            continue;
        }

        if (user == current_user) {
            int easy  = std::stoi(wEasy);
            int med   = std::stoi(wMed);
            int hard  = std::stoi(wHard);
            int lo    = std::stoi(losses);
            int tr    = std::stoi(tries);

            if (won) {
                if (difficulty == "easy") ++easy;
                else if (difficulty == "medium") ++med;
                else if (difficulty == "hard") ++hard;
            } else {
                ++lo;
            }

            tr += tries_used;
            std::ostringstream os;
            os << user << ',' << easy << ',' << med << ',' << hard << ',' << lo << ',' << tr;
            l = os.str();
            break;
        }
    }

    std::ofstream out("shared/stats.csv");
    for (const std::string& l : lines) {
        out << l << '\n';
    }
}
void WordleGame::play(const std::string& word, const std::string& difficulty) {
    const int max_rows = 6;
    const int cols     = static_cast<int>(word.size());
    int attempts       = max_rows;

    std::array<std::array<char, 5>, 6> grid{};
    std::array<std::array<int, 5>, 6> state{};
    std::array<int, 26> key_state{};

    int row = 0;

    constexpr int SCREEN_WIDTH = 80;
    auto indent = [&](int width) {
        int spaces = (SCREEN_WIDTH - width) / 2;
        if (spaces < 0) spaces = 0;
        return std::string(spaces, ' ');
    };

    auto redraw = [&]() {
        std::cout << CLEAR_SCREEN;
        int board_width = cols * 4;
        for (int r = 0; r < max_rows; ++r) {
            std::cout << indent(board_width);
            for (int c = 0; c < cols; ++c) {
                char ch = grid[r][c] ? static_cast<char>(std::toupper(grid[r][c])) : ' ';
                switch (state[r][c]) {
                    case 3: std::cout << GREEN;  break;
                    case 2: std::cout << YELLOW; break;
                    case 1: std::cout << GRAY;   break;
                    default: /* no color */;    break;
                }
                std::cout << "[" << ch << "] " << RESET;
            }
            std::cout << "\n\n";
        }
        auto print_row = [&](const std::string& keys) {
            int width = static_cast<int>(keys.size()) * 4;
            std::cout << indent(width);
            for(char k : keys) {
                int st = key_state[k - 'a'];
                switch(st) {
                    case 3: std::cout << GREEN; break;
                    case 2: std::cout << YELLOW; break;
                    case 1: std::cout << GRAY; break;
                    default: break;
                }
                std::cout << "[" << static_cast<char>(std::toupper(k)) << "] " << RESET;
            }
            std::cout << '\n';
        };
        print_row("qwertyuiop");
        print_row("asdfghjkl");
        print_row("zxcvbnm");
        std::string att = "Attempts left: " + std::to_string(max_rows - row);
        std::cout << '\n' << indent(static_cast<int>(att.size())) << att << "\n\n";
    };

    while (attempts > 0) {
        redraw();

        std::string guess;
        do {
            std::string prompt = "Enter your guess: ";
            std::cout << indent(static_cast<int>(prompt.size())) << prompt;
            std::cin >> guess;
            if (guess.size() != cols) {
                std::string msg = "→ Please enter a " + std::to_string(cols)
                                 + "-letter word.";
                std::cout << indent(static_cast<int>(msg.size())) << msg << '\n';
            }
        } while (guess.size() != cols);

        for (int c = 0; c < cols; ++c) {
            grid[row][c] = static_cast<char>(std::tolower(guess[c]));
        }

        if (guess == word) {
            for (int c = 0; c < cols; ++c) {
                state[row][c] = 3;
                key_state[guess[c]-'a'] = std::max(key_state[guess[c]-'a'], 3);
            }
            redraw();
            {
                std::string msg = "You win!";
                std::cout << indent(static_cast<int>(msg.size())) << msg << '\n';
            }
            update_stats(true, row + 1, difficulty);
            return;
        }

        for (int c = 0; c < cols; ++c) {
            char g = static_cast<char>(std::tolower(guess[c]));
            if (g == word[c]) {
                state[row][c] = 3;
                key_state[g-'a'] = std::max(key_state[g-'a'], 3);
            } else if (word.find(g) != std::string::npos) {
                state[row][c] = 2;
                key_state[g-'a'] = std::max(key_state[g-'a'], 2);
            } else {
                state[row][c] = 1;
                key_state[g-'a'] = std::max(key_state[g-'a'], 1);
            }
        }

        ++row;
        --attempts;
        clear_console();
    }

    redraw();
    {
        std::string msg = "Out of tries! The word was: " + word;
        std::cout << indent(static_cast<int>(msg.size())) << msg << '\n';
    }
    update_stats(false, max_rows, difficulty);
}