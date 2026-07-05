#ifndef CLASS_H
#define CLASS_H

#include "struct.h"
#include <vector>
#include <stack>
#include <string>

// 棋盘类
class Board {
private:
    std::stack<std::vector<std::vector<int>>> history;
    std::vector<std::vector<int>> start_board;

public:
    BoardType type; // 棋盘类型
    int step;
    std::vector<std::vector<int>> board; // 棋盘状态
    int peg_count; // 剩余棋子数

    // 构造函数
    Board();
    void init_board(); // 初始化棋盘
    bool check_move_possible(int x1, int y1, int x2, int y2); // 判断移动是否合法
    bool make_move(int x1, int y1, int x2, int y2); // 执行移动
    void get_remaining(); // 获取剩余棋子数
    void undo(); // 撤销操作
    void reset(); // 重置棋盘
    GameState check_game_state(); // 检查游戏状态
    std::vector<Move> get_valid_moves(); // 获取所有合法移动
    Move get_best_move(); // AI提示最佳移动
    int calculate_clustering_score(const Move& move);
    int evaluate_future_mobility(const Move& move);
    void printBoardState() const;
    int peg_connectivity(const Move& move);
};

// 游戏控制类
class GameController {
private:
    Board board;
    GameState state;
    int score;
    int moveCount;

public:
    void startGame() {}
    void processInput() {}
    void render() {}
    void showHint() {}
    void saveGame() {}
    void loadGame() {}
};

#endif