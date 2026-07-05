#ifndef STRUCT_H
#define STRUCT_H

// 坐标结构
struct Position {
    int x, y;
};

// 移动结构
struct Move {
    Position from; // 起始位置
    Position to;   // 目标位置
    Position gap;  // 被跳过的位置
};

// GameState 枚举名
enum GameState {
    GAME_PLAYING,  
    GAME_WIN,      
    GAME_LOSE,    
    GAME_PAUSED 
};

// 棋盘类型枚举
enum BoardType {
    STANDARD, // 经典33孔棋盘
    DIAMOND,  // 菱形棋盘
    CROSS,    // 十字形棋盘
    SQUARE    // 正方形棋盘
};

#endif 