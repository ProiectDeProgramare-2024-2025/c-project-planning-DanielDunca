#ifndef INTERFACE_H
#define INTERFACE_H
#include <string>
#include <vector>
void menu();
void play();
void about();
void stats();
void exit();
void clear_console();
void leaderboard();
int promptForChoice(const std::string& prompt, const std::vector<int>& validOptions);
#endif //INTERFACE_H
