#include "ui.h"
#include "config.h"
#include "class.h"
#include "show.h"
#include <graphics.h>
#include <string>

UIController::UIController() : currentState(UI_MAIN_MENU) {}

void UIController::setState(UIState state) {
    currentState = state;
}

UIState UIController::getState() const {
    return currentState;
}

void UIController::renderCurrentState() {
  
}