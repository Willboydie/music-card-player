#include "App.hpp"


void AppController::run() {

    ButtonManager buttonManager;
    ScreenManager screenManager;
    
    auto stateMachine = buildStateMachine(screenManager);
    
    while (true) {
        
        if (buttonManager.isUpButtonPressed()) {
            stateMachine->onUpButton();
        }
        if (buttonManager.isDownButtonPressed()) {
            stateMachine->onDownButton();
        }
        if (buttonManager.isSelectButtonPressed()) {
            stateMachine->onSelectButton();
        }
        if (buttonManager.isBackButtonPressed()) {
            stateMachine->onBackButton();
        }

        screenManager.update();
    }
};