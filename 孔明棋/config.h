#ifndef CONFIG_H
#define CONFIG_H

#include <graphics.h>

// 窗口尺寸
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const char* WINDOW_TITLE;

// 棋盘参数
extern const int BOARD_SIZE;
extern const int CELL_SIZE;
extern const int BOARD_OFFSET_X;
extern const int BOARD_OFFSET_Y;

// 颜色 - 调整为贴近 PPT 的暖色调配色
extern const COLORREF BACKGROUND_COLOR;       // 界面背景（暖桃色）
extern const COLORREF BOARD_PANEL_COLOR;      // 棋盘面板背景（浅米）
extern const COLORREF BOARD_BORDER_COLOR;     // 棋盘边框颜色
extern const COLORREF CELL_BG_COLOR;          // 单元格背景（浅米）
extern const COLORREF PEG_COLOR;              // 棋子主色（珊瑚橙）
extern const COLORREF PEG_SHADOW_COLOR;       // 棋子阴影色
extern const COLORREF EMPTY_PEG_COLOR;        // 空位颜色（白）
extern const COLORREF SELECTED_PEG_COLOR;     // 选中棋子颜色
extern const COLORREF VALID_MOVE_COLOR;       // 有效移动提示色
extern const COLORREF TEXT_COLOR;             // 常规文字颜色

// 按钮颜色
extern const COLORREF BUTTON_PINK;
extern const COLORREF BUTTON_PINK_HOVER;
extern const COLORREF BUTTON_ORANGE;
extern const COLORREF BUTTON_ORANGE_HOVER;
extern const COLORREF BUTTON_GREEN;
extern const COLORREF BUTTON_GREEN_HOVER;
extern const COLORREF BUTTON_BLUE;
extern const COLORREF BUTTON_BLUE_HOVER;

// 按钮尺寸（菜单）
extern const int MENU_BUTTON_WIDTH;
extern const int MENU_BUTTON_HEIGHT;
extern const int MENU_BUTTON_MARGIN;

// 其它文字大小
extern const int MENU_TITLE_SIZE;
extern const int MENU_OPTION_SIZE;
extern const int INFO_FONT_SIZE;

#endif 