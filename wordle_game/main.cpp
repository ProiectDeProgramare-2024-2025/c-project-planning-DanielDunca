#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "interface.h"
#include <ctime>
#include "check_guess.h"
#include "../shared/colors.h"

int main(int argc, char* argv[]) {
    srand(time(0));
#ifdef _WIN32
    enableANSIColors();
#endif
    int screenWidth = 80;
    if (argc > 1) {
        try {
            screenWidth = std::stoi(argv[1]);
        } catch (...) {
            // keep default on invalid input
        }
    }
    const int SCREEN_WIDTH = screenWidth;
    auto indent = [&](int width) {
        int spaces = (SCREEN_WIDTH - width) / 2;
        if (spaces < 0) spaces = 0;
        return std::string(spaces, ' ');
    };
    if (std::filesystem::exists("shared/session.txt")) {
        int choice = -1;
        while (true) {
            clear_console();
            menu();
            choice = promptForChoice("", {0,1,2,3,4});
            switch (choice) {
                case 1:{
                    clear_console();
                    play();
                    int difficulty = promptForChoice("Select " UNDERLINE "option" RESET ": ", {0,1,2,3});
                    switch (difficulty) {
                        case 1:{
                            int randomNumbEz = rand() % 199 + 1;
                            std::cout << randomNumbEz << std::endl;
                            std::string myTextEz;
                            std::ifstream easy("shared/easy.txt");
                            for (int i = 1; i <= randomNumbEz; i++) {
                                getline(easy, myTextEz);

                            }
                            std::ofstream secreteasy("shared/secret.txt");
                            secreteasy<<  myTextEz;
                            secreteasy.close();
                            easy.close();
                            WordleGame game;
                            game.play(myTextEz, "easy");
                            {
                                std::string msg = "Press any key to go back.";
                                std::cout << indent(static_cast<int>(msg.size()))
                                          << "Press " BOLD "any" RESET " key to go " RED "back" RESET ".";
                            }
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cin.get();
                            clear_console();
                            break;
                        }
                        case 2: {
                            int randomNumbMed = rand() % 199 + 1;
                            std::cout << randomNumbMed << std::endl;
                            std::string myTextMed;
                            std::ifstream medium("shared/medium.txt");
                            for (int i = 1; i <= randomNumbMed; i++) {
                                getline(medium, myTextMed);

                            }
                            std::ofstream secretmed("shared/secret.txt");
                            secretmed<<  myTextMed;
                            secretmed.close();
                            medium.close();
                            WordleGame game;
                            game.play(myTextMed, "medium");
                            {
                                std::string msg = "Press any key to go back.";
                                std::cout << indent(static_cast<int>(msg.size()))
                                          << "Press " BOLD "any" RESET " key to go " RED "back" RESET ".";
                            }
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cin.get();
                            clear_console();
                            break;
                        }
                        case 3: {
                            int randomNumbHard = rand() % 199 + 1;
                            std::cout << randomNumbHard << std::endl;
                            std::string myTextHard;
                            std::ifstream hard("shared/hard.txt");
                            for (int i = 1; i <= randomNumbHard; i++) {
                                getline(hard, myTextHard);

                            }
                            std::ofstream secrethard("shared/secret.txt");
                            secrethard<<  myTextHard;
                            secrethard.close();
                            hard.close();
                            WordleGame game;
                            game.play(myTextHard, "hard");
                            {
                                std::string msg = "Press any key to go back.";
                                std::cout << indent(static_cast<int>(msg.size()))
                                          << "Press " BOLD "any" RESET " key to go " RED "back" RESET ".";
                            }
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cin.get();
                            clear_console();
                            break;
                        }
                        case 0:
                            break;
                    }
                    break;
                }
                case 2:
                    clear_console();
                    about();
                    promptForChoice("Press " BOLD "0" RESET " to go " RED "back" RESET ": ", {0});
                    break;
                case 3:
                    clear_console();
                    stats();
                    promptForChoice("Press " BOLD "0" RESET " to go " RED "back" RESET ": ", {0});
                    break;
                case 4:
                    clear_console();
                    leaderboard();
                    promptForChoice("Press " BOLD "0" RESET " to go " RED "back" RESET ": ", {0});
                    break;
                case 0:
                    exit(0);
            }
        }
    }
    else {
        std::cout << UNDERLINE "No user is currently logged in." RESET << std::endl;
        return 0;
    }

    return 0;
}