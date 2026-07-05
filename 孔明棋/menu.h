
#ifndef MENU_H
#define MENU_H

#include "struct.h"
#include <string>
#include <vector>

// 按钮类型
enum ButtonType {
    BTN_NONE,           // 无操作
    BTN_CLASSIC_BOARD,
    BTN_DIAMOND_BOARD,
    BTN_SQUARE_BOARD,
    BTN_CROSS_BOARD,
    BTN_CLASSIC_PLAY,
    BTN_DIAMOND_PLAY,
    BTN_SQUARE_PLAY,
    BTN_CROSS_PLAY,
    BTN_BACK_TO_MAIN,
    BTN_RETURN_MODE,
    BTN_RETURN_MAIN,
    BTN_UNDO,
    BTN_HINT,
    BTN_RESTART,
    BTN_START_GAME,
    BTN_EXIT_GAME,
    BTN_GAME_HELP,      // 新：进入说明页面

    // 导航/占位类型
    BTN_NAV_LEFT,    // 模式选择页左箭头
    BTN_NAV_RIGHT,   // 模式选择页右箭头
    BTN_PREVIEW      // 模式预览占位
};

enum MenuState {
    MAIN_MENU,
    MODE_SELECTION,
    GAME_SCREEN,
    GAME_HELP
};

struct MenuButton {
    int x = 0, y = 0, width = 0, height = 0;
    std::string text = "";
    ButtonType type = BTN_START_GAME;
    bool hover = false;
};

class Menu {
private:
    MenuState currentState;
    std::vector<MenuButton> buttons;
    BoardType selectedBoardType; // MODE_SELECTION 下当前选择

    void createMainMenu();
    void createModeSelection();
    void createGameScreen();
    void createGameHelp();

public:
    Menu();
    MenuState getState() const;
    void setState(MenuState state);
    BoardType getSelectedBoardType() const;
    void updateButtons();
 
    void render(bool clearBackground = true);
    ButtonType checkButtonClick(int mouseX, int mouseY);
    void updateButtonHover(int mouseX, int mouseY);
    void handleButtonClick(ButtonType type);
};

#endif 