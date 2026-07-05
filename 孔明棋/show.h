#ifndef SHOW_H
#define SHOW_H

#include <string>
#include <vector>


class Board;
struct Move;

class Render {
private:
    int buttonY;
public:
    Render();
    void renderGameBoard(const Board& board);
    void renderGameInfo(const Board& board);
    void renderGameHelp();
};

#endif // SHOW_H