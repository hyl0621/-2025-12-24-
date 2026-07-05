#include "config.h"

// 窗口尺寸
const int WINDOW_WIDTH = 1365;
const int WINDOW_HEIGHT = 768;
const char* WINDOW_TITLE = "孔明棋";

// 棋盘参数
const int BOARD_SIZE = 7;
const int CELL_SIZE = 64;          
const int BOARD_OFFSET_X = 100;
const int BOARD_OFFSET_Y = 150;

// 颜色（贴近 PPT 风格）
const COLORREF BACKGROUND_COLOR = RGB(245, 205, 185);      // 暖桃米色背景
const COLORREF BOARD_PANEL_COLOR = RGB(255, 242, 235);     // 棋盘面板（更浅）
const COLORREF BOARD_BORDER_COLOR = RGB(60, 90, 140);      // 深蓝边框
const COLORREF CELL_BG_COLOR = BOARD_PANEL_COLOR;
const COLORREF PEG_COLOR = RGB(210, 90, 60);               // 珊瑚橙（棋子）
const COLORREF PEG_SHADOW_COLOR = RGB(170, 110, 90);       // 棋子阴影
const COLORREF EMPTY_PEG_COLOR = RGB(252, 248, 240);       // 空位（接近白）
const COLORREF SELECTED_PEG_COLOR = RGB(230, 80, 80);      // 选中棋子（鲜红）
const COLORREF VALID_MOVE_COLOR = RGB(170, 220, 170);      // 有效移动提示色（绿色）
const COLORREF TEXT_COLOR = RGB(30, 30, 30);               // 常规黑色文字

// 按钮颜色（尽量接近期望 PPT 色块）
const COLORREF BUTTON_PINK = RGB(234, 90, 118);
const COLORREF BUTTON_PINK_HOVER = RGB(250, 110, 140);
const COLORREF BUTTON_ORANGE = RGB(230, 130, 50);
const COLORREF BUTTON_ORANGE_HOVER = RGB(245, 150, 70);
const COLORREF BUTTON_GREEN = RGB(120, 190, 70);
const COLORREF BUTTON_GREEN_HOVER = RGB(150, 210, 90);
const COLORREF BUTTON_BLUE = RGB(80, 120, 200);
const COLORREF BUTTON_BLUE_HOVER = RGB(110, 150, 230);

// 菜单按钮尺寸
const int MENU_BUTTON_WIDTH = 300;
const int MENU_BUTTON_HEIGHT = 80;
const int MENU_BUTTON_MARGIN = 30;

// 字体大小
const int MENU_TITLE_SIZE = 48;
const int MENU_OPTION_SIZE = 28;
const int INFO_FONT_SIZE = 22;