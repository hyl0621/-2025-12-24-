
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include "struct.h"
#include "class.h"
#include "config.h"
#include "show.h"
#include "menu.h"

using namespace std;

int main() {
    // 初始化 EasyX 窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);
    SetWindowText(GetHWnd(), WINDOW_TITLE);
    setbkcolor(BACKGROUND_COLOR);
    BeginBatchDraw();
    cleardevice();

    // 模块实例
    Render render;
    Board board;
    Menu menu;

    // 记录上一次用于初始化的棋盘类型（用于检测用户是否在模式选择中更改）
    BoardType lastSelectedBoardType = STANDARD;

    // 运行时状态
    Position selectedPeg = { -1, -1 };
    vector<Move> validMoves;
    bool gameActive = false;
    bool exitProgram = false;
    GameState currentGameState = GAME_PLAYING;

    // 初始界面为主菜单
    menu.setState(MAIN_MENU);

    // 启动时清空鼠标队列并防抖（避免启动瞬间残留点击切页）
    while (MouseHit()) GetMouseMsg();
    const DWORD IGNORE_MS = 200; // 启动后忽略前 200ms 的点击（可设 0 关闭）
    DWORD ignore_until = GetTickCount() + IGNORE_MS;

    cout << "孔明棋 程序启动" << endl;

    while (!exitProgram) {
        // 每帧清屏（具体页面由 render/menu 决定是否覆盖）
        cleardevice();

        // 读取菜单当前状态（同步）
        MenuState currentState = menu.getState();

        try {
            // 处理鼠标消息
            while (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();

                // 更新 hover
                menu.updateButtonHover(msg.x, msg.y);

                if (msg.uMsg == WM_LBUTTONDOWN) {
                    // 启动防抖期间忽略点击
                    if (GetTickCount() < ignore_until) {
                        continue;
                    }

                    // 识别点击到的按钮
                    ButtonType btnType = menu.checkButtonClick(msg.x, msg.y);

                    // 如果点击退出按钮，结束程序循环
                    if (btnType == BTN_EXIT_GAME) {
                        exitProgram = true;
                        break;
                    }

                    // 仅当命中按钮时让 menu 处理
                    if (btnType != BTN_NONE) {
                        menu.handleButtonClick(btnType);
                        // 立即同步菜单状态
                        currentState = menu.getState();
                    }

                    // 如果当前在游戏界面，主循环还需处理一些按钮行为
                    if (currentState == GAME_SCREEN) {
                        switch (btnType) {
                        case BTN_RETURN_MODE:
                            menu.setState(MODE_SELECTION);
                            currentState = menu.getState();
                            gameActive = false;
                            currentGameState = GAME_PLAYING;
                            selectedPeg = { -1, -1 };
                            validMoves.clear();
                            break;

                        case BTN_RETURN_MAIN:
                            menu.setState(MAIN_MENU);
                            currentState = menu.getState();
                            gameActive = false;
                            currentGameState = GAME_PLAYING;
                            selectedPeg = { -1, -1 };
                            validMoves.clear();
                            break;

                        case BTN_UNDO:
                            board.undo();
                            currentGameState = board.check_game_state();
                            break;

                        case BTN_HINT: {
                            Move bestMove = board.get_best_move();
                            if (bestMove.from.x != -1) {
                                selectedPeg = bestMove.from;
                                validMoves.clear();
                                validMoves.push_back(bestMove);
                            }
                            break;
                        }

                        case BTN_RESTART:
                            board.init_board();
                            selectedPeg = { -1, -1 };
                            validMoves.clear();
                            board.step = 0;
                            board.get_remaining();
                            gameActive = true;
                            currentGameState = GAME_PLAYING;
                            break;

                        default:
                            break;
                        }
                    }

                    // 若当前为游戏界面并且点击位置在棋盘区域，处理选子 / 落子逻辑
                    if (currentState == GAME_SCREEN) {
                        if (gameActive && currentGameState == GAME_PLAYING) {
                            int boardStartX = WINDOW_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2;
                            int boardStartY = 150;
                            int gridX = (msg.x - boardStartX) / CELL_SIZE;
                            int gridY = (msg.y - boardStartY) / CELL_SIZE;

                            if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE) {
                                if (selectedPeg.x == -1) {
                                    // 选中棋子（如果该格存在棋子）
                                    if (gridX < (int)board.board.size() && gridY < (int)board.board[0].size()) {
                                        if (board.board[gridX][gridY] == 1) {
                                            selectedPeg = { gridX, gridY };
                                            validMoves.clear();
                                            vector<Move> allMoves = board.get_valid_moves();
                                            for (const auto& move : allMoves) {
                                                if (move.from.x == gridX && move.from.y == gridY) {
                                                    validMoves.push_back(move);
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    // 尝试落子
                                    bool moved = false;
                                    for (const auto& move : validMoves) {
                                        if (move.to.x == gridX && move.to.y == gridY) {
                                            if (board.make_move(selectedPeg.x, selectedPeg.y, gridX, gridY)) {
                                                moved = true;
                                                break;
                                            }
                                        }
                                    }
                                    selectedPeg = { -1, -1 };
                                    validMoves.clear();

                                    if (moved) {
                                        currentGameState = board.check_game_state();
                                    }
                                }
                            }
                        }
                    }
                } 
            } 
            // 键盘处理：ESC 退出
            if (_kbhit()) {
                int key = _getch();
                if (key == 27) exitProgram = true;
            }

            // 渲染分支（ GAME_SCREEN 的渲染顺序：棋盘 -> overlay -> 信息面板 -> 高亮 -> 结果提示）
            switch (currentState) {
            case MAIN_MENU:
                menu.render(true);
                break;

            case MODE_SELECTION:
                menu.render(true);
                break;

            case GAME_HELP:
                menu.render(true);
                break;

            case GAME_SCREEN:
                // 进入游戏时若尚未初始化，或用户在模式选择中改变了选择，则重新初始化
            {
                BoardType currentChoice = menu.getSelectedBoardType();
                if (!gameActive || lastSelectedBoardType != currentChoice) {
                    board.type = currentChoice;
                    board.init_board();
                    lastSelectedBoardType = currentChoice;
                    gameActive = true;
                    currentGameState = GAME_PLAYING;
                    selectedPeg = { -1, -1 };
                    validMoves.clear();
                }
            }

            // 1) 绘制棋盘（底层）
            render.renderGameBoard(board);

            // 2) 绘制菜单的 overlay，但不覆盖信息面板
            menu.render(false);

            // 3) 绘制信息面板，放在 overlay 之上确保可见
            render.renderGameInfo(board);

            // 4) 绘制选中与有效移动高亮（最上层）
            if (selectedPeg.x != -1 && currentGameState == GAME_PLAYING) {
                setfillcolor(VALID_MOVE_COLOR);
                for (const auto& move : validMoves) {
                    // 计算圆心与半径（和棋子绘制使用的一致）
                    int centerX = (WINDOW_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2) + move.to.x * CELL_SIZE + CELL_SIZE / 2;
                    int centerY = 150 + move.to.y * CELL_SIZE + CELL_SIZE / 2;
                    int radius = CELL_SIZE / 2 - 8;

                    // 阴影（与棋子阴影一致的偏移）
                    setfillcolor(PEG_SHADOW_COLOR);
                    solidcircle(centerX + 4, centerY + 6, radius);

                    // 高亮圆（颜色使用 VALID_MOVE_COLOR）
                    setfillcolor(VALID_MOVE_COLOR);
                    solidcircle(centerX, centerY, radius);

                    // 可选：画一圈边框使高亮更清晰
                    setlinecolor(BOARD_BORDER_COLOR);
                    circle(centerX, centerY, radius);
                }
               
                int centerX = (WINDOW_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2) + selectedPeg.x * CELL_SIZE + CELL_SIZE / 2;
                int centerY = 150 + selectedPeg.y * CELL_SIZE + CELL_SIZE / 2;
                int radius = CELL_SIZE / 2 - 8;
                setfillcolor(SELECTED_PEG_COLOR);
                fillcircle(centerX, centerY, radius);
                setlinecolor(TEXT_COLOR);
                
            }

            // 5) 游戏结果提示（统一绘制到棋盘下方，居中，带阴影）
            {
                int boardStartX = WINDOW_WIDTH / 2 - (BOARD_SIZE * CELL_SIZE) / 2;
                int boardStartY = 150;
                int boardW = BOARD_SIZE * CELL_SIZE;

                string msg = "";
                COLORREF color = RGB(0, 0, 0);

                if (currentGameState == GAME_WIN) {
                    msg = "恭喜你，通关成功！";
                    color = RGB(0, 160, 0);
                }
                else if (currentGameState == GAME_LOSE) {
                    msg = "游戏失败，请重试";
                    color = RGB(200, 0, 0);
                }

                if (!msg.empty()) {
                    settextstyle(28, 0, "楷体");
                    setbkmode(TRANSPARENT);
                    int tw = textwidth(msg.c_str());
                    int th = textheight(msg.c_str());
                    int msgX = boardStartX + (boardW - tw) / 2;
                    int msgY = boardStartY + boardW + 12;

                    // 阴影
                    settextcolor(RGB(120, 120, 120));
                    outtextxy(msgX + 2, msgY + 2, msg.c_str());
                    // 主色
                    settextcolor(color);
                    outtextxy(msgX, msgY, msg.c_str());
                }
            }
            break;
            } 
        } 
        catch (const exception& e) {
            cout << "异常: " << e.what() << endl;
            settextcolor(RED);
            settextstyle(20, 0, "宋体");
            setbkmode(TRANSPARENT);
            outtextxy(100, 100, "发生异常");
        }

        // 刷新画面
        FlushBatchDraw();
        Sleep(30);
    } 

    // 退出清理
    EndBatchDraw();
    closegraph();
    return 0;
}