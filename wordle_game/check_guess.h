#ifndef CHECK_GUESS_H
#define CHECK_GUESS_H

#include <string>

class WordleGame {
public:
    void play(const std::string& word, const std::string& difficulty);
private:
    static void update_stats(bool won, int tries_used, const std::string& difficulty);
};

#endif // CHECK_GUESS_H