#ifndef UI_H
#define UI_H

// 界面状态枚举 
enum UIState {
    UI_MAIN_MENU,      // 主菜单
    UI_MODE_SELECTION, // 模式选择
    UI_PLAYING,        // 游戏中
    UI_INSTRUCTIONS    // 游戏说明
};

// 界面控制器类
class UIController {
private:
    UIState currentState;
public:
    UIController();
    void setState(UIState state);
    UIState getState() const;
    void renderCurrentState();
};

#endif 