#include "menu.h"
#include "config.h"
#include "class.h"
#include <graphics.h>
#include <string>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cstdlib> // 

//  辅助绘制函数

// 绘制圆角按钮（文本居中，带阴影）
static void DrawRoundedButtonRect(int x, int y, int w, int h, const std::string& text, COLORREF bg, COLORREF border, COLORREF txtColor, int radius = 18)
{
    int sx = 4, sy = 6;
    setfillcolor(RGB(200, 200, 200));
    solidroundrect(x + sx, y + sy, x + w + sx, y + h + sy, radius, radius);

    setfillcolor(bg);
    solidroundrect(x, y, x + w, y + h, radius, radius);

    setlinecolor(border);
    setlinestyle(PS_SOLID, 2);
    roundrect(x, y, x + w, y + h, radius, radius);

    if (!text.empty()) {
        setbkmode(TRANSPARENT);
        settextcolor(txtColor);
        if (w >= 300 && h >= 60) settextstyle(MENU_TITLE_SIZE, 0, "楷体");
        else if (h >= 60) settextstyle(MENU_OPTION_SIZE, 0, "楷体");
        else settextstyle(INFO_FONT_SIZE, 0, "宋体");
        int tw = textwidth(text.c_str());
        int th = textheight(text.c_str());
        outtextxy(x + (w - tw) / 2, y + (h - th) / 2, text.c_str());
    }
}

// 三角箭头
static void DrawTriangleAt(int cx, int cy, int half, bool left, COLORREF fill, COLORREF border)
{
    POINT pts[3];
    if (left) {
        pts[0] = { cx - half, cy };
        pts[1] = { cx + half, cy - half };
        pts[2] = { cx + half, cy + half };
    }
    else {
        pts[0] = { cx + half, cy };
        pts[1] = { cx - half, cy - half };
        pts[2] = { cx - half, cy + half };
    }
    setfillcolor(fill);
    solidpolygon(pts, 3);
    setlinecolor(border);
    polygon(pts, 3);
}

// 小棋盘预览
static void DrawSmallPreview(int centerX, int centerY, BoardType type)
{
    int r = 10;
    int gap = r * 3;
    int map[7][7];

    switch (type) {
    case STANDARD: {
        int m[7][7] = {
            {-1,-1,1,1,1,-1,-1},
            {-1,-1,1,1,1,-1,-1},
            {1,1,1,1,1,1,1},
            {1,1,1,0,1,1,1},
            {1,1,1,1,1,1,1},
            {-1,-1,1,1,1,-1,-1},
            {-1,-1,1,1,1,-1,-1}
        }; memcpy(map, m, sizeof(map));
    } break;
    case DIAMOND: {
        int m[7][7] = {
            {-1,-1,-1,1,-1,-1,-1},
            {-1,-1,1,1,1,-1,-1},
            {-1,1,1,1,1,1,-1},
            {1,1,1,0,1,1,1},
            {-1,1,1,1,1,1,-1},
            {-1,-1,1,1,1,-1,-1},
            {-1,-1,-1,1,-1,-1,-1}
        }; memcpy(map, m, sizeof(map));
    } break;
    case SQUARE: {
        int m[7][7] = {
            {1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1},
            {1,1,1,0,1,1,1},
            {1,1,0,0,0,1,1},
            {1,1,1,0,1,1,1},
            {1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1}
        }; memcpy(map, m, sizeof(map));
    } break;
    case CROSS: {
        int m[7][7] = {
            {-1,-1,1,1,1,-1,-1},
            {-1,-1,1,1,1,-1,-1},
            {1,1,1,0,1,1,1},
            {1,1,0,0,0,1,1},
            {1,1,1,0,1,1,1},
            {-1,-1,1,1,1,-1,-1},
            {-1,-1,1,1,1,-1,-1}
        }; memcpy(map, m, sizeof(map));
    } break;
    default:
        return;
    }

    for (int y = 0; y < 7; ++y) {
        for (int x = 0; x < 7; ++x) {
            if (map[y][x] == -1) continue;
            int cx = centerX + (x - 3) * gap;
            int cy = centerY + (y - 3) * gap;
            setfillcolor(PEG_SHADOW_COLOR);
            solidcircle(cx + 4, cy + 6, r);
            setfillcolor(map[y][x] == 1 ? PEG_COLOR : EMPTY_PEG_COLOR);
            solidcircle(cx, cy, r);
        }
    }
}

// 主菜单模糊背景
static void DrawBlurBackgroundForMainMenu(int centerX, int centerY)
{
    int r = 60;
    int gap = r * 2;
    int map[7][7] = {
        {-1,-1,1,1,1,-1,-1},
        {-1,-1,1,1,1,-1,-1},
        {1,1,1,1,1,1,1},
        {1,1,1,0,1,1,1},
        {1,1,1,1,1,1,1},
        {-1,-1,1,1,1,-1,-1},
        {-1,-1,1,1,1,-1,-1}
    };
    for (int y = 0; y < 7; ++y) {
        for (int x = 0; x < 7; ++x) {
            if (map[y][x] == -1) continue;
            int cx = centerX + (x - 3) * gap;
            int cy = centerY + (y - 3) * gap;
            setfillcolor(RGB(240, 210, 205));
            solidcircle(cx, cy, r);
        }
    }
}

//  Menu 实现 
Menu::Menu() : currentState(MAIN_MENU), selectedBoardType(STANDARD)
{
    updateButtons();
}

MenuState Menu::getState() const { return currentState; }
void Menu::setState(MenuState state) { currentState = state; updateButtons(); }
BoardType Menu::getSelectedBoardType() const { return selectedBoardType; }

void Menu::updateButtons()
{
    buttons.clear();
    int centerX = WINDOW_WIDTH / 2;

    switch (currentState) {
    case MAIN_MENU:
        buttons.push_back({ centerX - 150, 120, 300, 80, "孔明棋", BTN_START_GAME });
        buttons.push_back({ centerX - MENU_BUTTON_WIDTH / 2, 240, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "模式选择", BTN_START_GAME });
        buttons.push_back({ centerX - MENU_BUTTON_WIDTH / 2, 340, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "游戏说明", BTN_GAME_HELP });
        buttons.push_back({ centerX - MENU_BUTTON_WIDTH / 2, 440, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "退出游戏", BTN_EXIT_GAME });
        break;

    case MODE_SELECTION: {
        int previewCenterY = 260;
        int px = centerX - 120;
        int leftArrowX = px - 80;  // 与 render 中使用的一致
        int rightArrowX = px + 240 + 80;
        buttons.push_back({ centerX - MENU_BUTTON_WIDTH / 2, 40, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "模式选择", BTN_START_GAME });
        buttons.push_back({ px, previewCenterY - 120, 240, 240, "预览区域", BTN_PREVIEW });
        buttons.push_back({ leftArrowX - 30, previewCenterY - 40 - 40, 60, 80, "<", BTN_NAV_LEFT }); // place so center matches DrawTriangleAt
        buttons.push_back({ rightArrowX - 30, previewCenterY - 40 - 40, 60, 80, ">", BTN_NAV_RIGHT });
        buttons.push_back({ centerX - 150, previewCenterY + 140, 300, 70, "开始游玩", BTN_START_GAME });
        buttons.push_back({ centerX - 150, previewCenterY + 230, 300, 70, "返回主菜单", BTN_BACK_TO_MAIN });
    } break;

    case GAME_SCREEN:
        buttons.push_back({ 30, 30, 160, 50, "返回模式选择", BTN_RETURN_MODE });
        buttons.push_back({ 30, 100, 160, 50, "返回主菜单", BTN_RETURN_MAIN });
        {
            int rightX = WINDOW_WIDTH - 260;
            buttons.push_back({ rightX, 60, 220, 60, "剩余棋子数：", BTN_RESTART });
            buttons.push_back({ rightX, 140, 220, 60, "步数：", BTN_RESTART });
            buttons.push_back({ WINDOW_WIDTH - 260, 320, 180, 60, "撤销操作", BTN_UNDO });
            buttons.push_back({ WINDOW_WIDTH - 260, 420, 180, 60, "AI提示", BTN_HINT });
            buttons.push_back({ WINDOW_WIDTH - 260, 520, 180, 60, "重新开始", BTN_RESTART });
        }
        break;

    case GAME_HELP:
        buttons.push_back({ centerX - 150, 80, 300, 80, "游戏说明", BTN_GAME_HELP });
        buttons.push_back({ centerX - 100, WINDOW_HEIGHT - 100, 200, 50, "返回", BTN_BACK_TO_MAIN });
        break;
    }
}

void Menu::render(bool clearBackground)
{
    if (clearBackground) {
        if (currentState == MAIN_MENU) {
            setfillcolor(BACKGROUND_COLOR);
            solidrectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            DrawBlurBackgroundForMainMenu(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 20);
        }
        else {
            setfillcolor(BACKGROUND_COLOR);
            solidrectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
    }

    if (!clearBackground) {
        if (currentState == GAME_SCREEN) {
            for (const auto& b : buttons) {
                if (b.type == BTN_RETURN_MODE || b.type == BTN_RETURN_MAIN ||
                    b.type == BTN_UNDO || b.type == BTN_HINT || b.type == BTN_RESTART) {
                    COLORREF bg = BUTTON_ORANGE;
                    COLORREF txt = RGB(10, 10, 40);
                    if (b.type == BTN_UNDO || b.type == BTN_HINT) { bg = BUTTON_GREEN; txt = RGB(0, 0, 0); }
                    if (b.type == BTN_RESTART) { bg = BUTTON_PINK; txt = RGB(255, 255, 255); }
                    DrawRoundedButtonRect(b.x, b.y, b.width, b.height, b.text, bg, BOARD_BORDER_COLOR, txt);
                }
            }
        }
        return;
    }

    if (currentState == MAIN_MENU) {
        setbkmode(TRANSPARENT);
        settextstyle(64, 0, "楷体");
        settextcolor(TEXT_COLOR);
        int tw = textwidth("孔明棋");
        outtextxy((WINDOW_WIDTH - tw) / 2, 120 + 10, "孔明棋");

        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, 240, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "模式选择", BUTTON_PINK, BOARD_BORDER_COLOR, RGB(255, 255, 255));
        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, 340, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "游戏说明", BUTTON_GREEN, BOARD_BORDER_COLOR, RGB(255, 255, 255));
        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, 440, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "退出游戏", BUTTON_BLUE, BOARD_BORDER_COLOR, RGB(255, 255, 255));
        return;
    }

    if (currentState == MODE_SELECTION) {
        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - MENU_BUTTON_WIDTH / 2, 40, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, "模式选择", BUTTON_PINK, BOARD_BORDER_COLOR, RGB(255, 255, 255));

        int px = WINDOW_WIDTH / 2 - 120;
        int py = 140;
        int pw = 240;
        int ph = 240;
        setfillcolor(BOARD_PANEL_COLOR);
        solidroundrect(px, py, px + pw, py + ph, 12, 12);
        setlinecolor(BOARD_BORDER_COLOR);
        roundrect(px, py, px + pw, py + ph, 12, 12);

        DrawSmallPreview(px + pw / 2, py + ph / 2, selectedBoardType);

       
        int leftCx = px - 80;
        int rightCx = px + pw + 80;
        int cy = py + ph / 2;
        DrawTriangleAt(leftCx, cy, 28, true, BUTTON_BLUE, BOARD_BORDER_COLOR);
        DrawTriangleAt(rightCx, cy, 28, false, BUTTON_BLUE, BOARD_BORDER_COLOR);

        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - 150, py + ph + 40, 300, 70, "开始游玩", BUTTON_ORANGE, BOARD_BORDER_COLOR, RGB(10, 10, 40));
        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - 150, py + ph + 120, 300, 70, "返回主菜单", BUTTON_ORANGE, BOARD_BORDER_COLOR, RGB(10, 10, 40));
        return;
    }

    if (currentState == GAME_SCREEN) {
        DrawRoundedButtonRect(30, 30, 160, 50, "返回模式选择", BUTTON_PINK, BOARD_BORDER_COLOR, RGB(255, 255, 255));
        DrawRoundedButtonRect(30, 100, 160, 50, "返回主菜单", BUTTON_ORANGE, BOARD_BORDER_COLOR, RGB(10, 10, 40));
        int rightX = WINDOW_WIDTH - 260;
        setfillcolor(RGB(235, 140, 50));
        solidroundrect(rightX - 10, 60 - 10, rightX + 220 + 10, 60 + 180 + 10, 24, 24);
        DrawRoundedButtonRect(WINDOW_WIDTH - 260, 320, 180, 60, "撤销操作", BUTTON_GREEN, BOARD_BORDER_COLOR, RGB(0, 0, 0));
        DrawRoundedButtonRect(WINDOW_WIDTH - 260, 420, 180, 60, "AI提示", BUTTON_GREEN, BOARD_BORDER_COLOR, RGB(0, 0, 0));
        DrawRoundedButtonRect(WINDOW_WIDTH - 260, 520, 180, 60, "重新开始", BUTTON_PINK, BOARD_BORDER_COLOR, RGB(255, 255, 255));
        return;
    }

    if (currentState == GAME_HELP) {
        settextstyle(28, 0, "宋体");
        settextcolor(TEXT_COLOR);
        setbkmode(TRANSPARENT);
        outtextxy(50, 150, "游戏规则：");
        outtextxy(80, 180, "1. 孔明棋是一种单人棋类游戏，起源于法国");
        outtextxy(80, 210, "2. 棋子只能跳过相邻的棋子到空位，被跳过的棋子被移除");
        outtextxy(80, 240, "3. 目标是通过一系列的跳跃，使棋盘上只剩下一个棋子");
        outtextxy(80, 270, "4. 最优解是最后只剩下一个棋子，并且位于棋盘中央");

        outtextxy(50, 320, "操作方法：");
        outtextxy(80, 350, "1. 点击选择一个棋子");
        outtextxy(80, 380, "2. 点击一个绿色格子进行跳跃");
        outtextxy(80, 410, "3. 棋子只能水平和垂直移动，不能斜向移动");

        outtextxy(50, 460, "游戏目标：");
        outtextxy(80, 490, "1.尽量用最少的步数完成游戏");
        outtextxy(80, 520, "2.挑战不同的棋盘布局，提升解谜能力");
        outtextxy(80, 550, "3.一些棋局未必能只剩下一个棋子，请尽可能使棋子数最少");
        DrawRoundedButtonRect(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 100, 200, 50, "返回", BUTTON_ORANGE, BOARD_BORDER_COLOR, RGB(10, 10, 40));
        return;
    }
}

ButtonType Menu::checkButtonClick(int mouseX, int mouseY)
{
    // 1) 如果当前是模式选择页，优先检测视觉三角区域（容差检测）
    if (currentState == MODE_SELECTION) {
        // 与 render 中使用的相同布局参数
        int centerX = WINDOW_WIDTH / 2;
        int px = centerX - 120;      // 预览面板左上 X（render 中同样使用）
        int py = 140;                // 预览面板左上 Y
        int pw = 240;
        int ph = 240;

        int leftCx = px - 80;        // render 中三角中心 X（左）
        int rightCx = px + pw + 80;  // render 中三角中心 X（右）
        int cy = py + ph / 2;        // 三角中心 Y

        // 判定容差（矩形或圆形）。使用半径 36 像素的圆形容差
        const int TRI_RADIUS = 36;
        int dxL = mouseX - leftCx;
        int dyL = mouseY - cy;
        if (dxL * dxL + dyL * dyL <= TRI_RADIUS * TRI_RADIUS) {
            return BTN_NAV_LEFT;
        }
        int dxR = mouseX - rightCx;
        int dyR = mouseY - cy;
        if (dxR * dxR + dyR * dyR <= TRI_RADIUS * TRI_RADIUS) {
            return BTN_NAV_RIGHT;
        }
    }

    // 2) 常规按钮矩形检测（保留现有按钮集合）
    for (const auto& button : buttons) {
        if (mouseX >= button.x && mouseX <= button.x + button.width &&
            mouseY >= button.y && mouseY <= button.y + button.height) {
            return button.type;
        }
    }

    // 3) 没有命中任何交互元素
    return BTN_NONE;
}
void Menu::updateButtonHover(int mouseX, int mouseY)
{
    for (auto& b : buttons) {
        b.hover = (mouseX >= b.x && mouseX <= b.x + b.width &&
            mouseY >= b.y && mouseY <= b.y + b.height);
    }
}

void Menu::handleButtonClick(ButtonType type)
{
    if (type == BTN_NONE) return;

    if (type == BTN_EXIT_GAME) {
        // 直接退出程序（安全退出）
        exit(0);
    }

    if (currentState == MODE_SELECTION) {
        if (type == BTN_NAV_RIGHT) {
            int idx = (static_cast<int>(selectedBoardType) + 1) % 4;
            selectedBoardType = static_cast<BoardType>(idx);
            updateButtons();
            return;
        }
        else if (type == BTN_NAV_LEFT) {
            int idx = static_cast<int>(selectedBoardType) - 1;
            if (idx < 0) idx += 4;
            selectedBoardType = static_cast<BoardType>(idx % 4);
            updateButtons();
            return;
        }
        else if (type == BTN_START_GAME) {
            setState(GAME_SCREEN);
            return;
        }
        else if (type == BTN_BACK_TO_MAIN) {
            setState(MAIN_MENU);
            return;
        }
    }

    if (currentState == MAIN_MENU && type == BTN_START_GAME) {
        selectedBoardType = STANDARD;
        setState(MODE_SELECTION);
        return;
    }

    if (type == BTN_GAME_HELP) {
        setState(GAME_HELP);
        return;
    }

    switch (type) {
    case BTN_CLASSIC_PLAY:
        selectedBoardType = STANDARD;
        setState(GAME_SCREEN);
        break;
    case BTN_BACK_TO_MAIN:
        setState(MAIN_MENU);
        break;
    case BTN_RETURN_MAIN:
        setState(MAIN_MENU);
        break;
    case BTN_RETURN_MODE:
        setState(MODE_SELECTION);
        break;
    default:
        break;
    }
}