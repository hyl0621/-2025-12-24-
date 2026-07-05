#include "show.h"
#include "class.h"
#include "config.h"
#include "menu.h"
#include <graphics.h>
#include <sstream>
#include <string>

using namespace std;

Render::Render() : buttonY(WINDOW_HEIGHT - 50) {}

// 绘制棋盘与棋子
void Render::renderGameBoard(const Board& board) {
    int boardStartX = WINDOW_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2;
    int boardStartY = 150;
    int panelPad = 8;

    // 背景面板
    setfillcolor(BOARD_PANEL_COLOR);
    solidrectangle(boardStartX - panelPad, boardStartY - panelPad,
        boardStartX + BOARD_SIZE * CELL_SIZE + panelPad,
        boardStartY + BOARD_SIZE * CELL_SIZE + panelPad);

    setlinecolor(BOARD_BORDER_COLOR);
    rectangle(boardStartX - panelPad, boardStartY - panelPad,
        boardStartX + BOARD_SIZE * CELL_SIZE + panelPad,
        boardStartY + BOARD_SIZE * CELL_SIZE + panelPad);

    // 绘制格子与棋子
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int x = boardStartX + i * CELL_SIZE;
            int y = boardStartY + j * CELL_SIZE;

            if (i < (int)board.board.size() && j < (int)board.board[i].size()) {
                if (board.board[i][j] == -1) continue;

                // 单元格背景
                setfillcolor(CELL_BG_COLOR);
                solidrectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);

                // 单元格边框
                setlinecolor(RGB(200, 200, 200));
                rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);

                // 棋子或空位
                if (board.board[i][j] == 1) {
                    int centerX = x + CELL_SIZE / 2;
                    int centerY = y + CELL_SIZE / 2;
                    int radius = CELL_SIZE / 2 - 8;

                    setfillcolor(PEG_SHADOW_COLOR);
                    solidcircle(centerX + 4, centerY + 6, radius);

                    setfillcolor(PEG_COLOR);
                    solidcircle(centerX, centerY, radius);

                    setfillcolor(RGB(255, 245, 235));
                    solidcircle(centerX - radius / 3, centerY - radius / 3, radius / 4);
                }
                else if (board.board[i][j] == 0) {
                    int centerX = x + CELL_SIZE / 2;
                    int centerY = y + CELL_SIZE / 2;
                    int radius = CELL_SIZE / 2 - 8;

                    setfillcolor(PEG_SHADOW_COLOR);
                    solidcircle(centerX + 4, centerY + 6, radius);

                    setfillcolor(EMPTY_PEG_COLOR);
                    solidcircle(centerX, centerY, radius);
                }
            }
        }
    }
}

// 右侧信息面板
void Render::renderGameInfo(const Board& board) {
    // 使用透明文本背景
    setbkmode(TRANSPARENT);

    int rightX = WINDOW_WIDTH - 260;
    int panelW = 220;
    int panelX = rightX;
    int panelY = 60;

    // 整个橘色外框
    setfillcolor(RGB(235, 140, 50));
    solidroundrect(panelX - 10, panelY - 10, panelX + panelW + 10, panelY + 180, 24, 24);
    setlinecolor(BOARD_BORDER_COLOR);
    roundrect(panelX - 10, panelY - 10, panelX + panelW + 10, panelY + 180, 24, 24);

    // 剩余棋子（蓝色内部）
    setfillcolor(BUTTON_BLUE);
    solidroundrect(panelX + 10, panelY + 10, panelX + panelW - 10, panelY + 60, 12, 12);
    settextstyle(20, 0, "楷体");
    settextcolor(RGB(255, 255, 255));
    outtextxy(panelX + 18, panelY + 18, "剩余棋子数：");

    char buf[32];
    sprintf_s(buf, "%d", board.peg_count);
    settextstyle(28, 0, "楷体");
    outtextxy(panelX + 120, panelY + 18, buf);

    // 步数
    setfillcolor(BUTTON_BLUE);
    solidroundrect(panelX + 10, panelY + 80, panelX + panelW - 10, panelY + 140, 12, 12);
    settextstyle(20, 0, "楷体");
    settextcolor(RGB(255, 255, 255));
    outtextxy(panelX + 18, panelY + 92, "步数：");
    sprintf_s(buf, "%d", board.step);
    settextstyle(28, 0, "楷体");
    outtextxy(panelX + 120, panelY + 92, buf);

    // 棋盘类型标题（显示在棋盘上方）
    std::string boardTitle;
    switch (board.type) {
    case STANDARD: boardTitle = "经典棋盘"; break;
    case DIAMOND: boardTitle = "菱形棋盘"; break;
    case SQUARE: boardTitle = "正方形棋盘"; break;
    case CROSS: boardTitle = "十字形棋盘"; break;
    default: boardTitle = "棋盘"; break;
    }

    int boardStartX = WINDOW_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2;
    int boardStartY = 150;
    settextstyle(40, 0, "楷体");
    settextcolor(TEXT_COLOR);
    int titleW = textwidth(boardTitle.c_str());
    outtextxy(boardStartX + (BOARD_SIZE * CELL_SIZE - titleW) / 2, boardStartY - 70, boardTitle.c_str());
}