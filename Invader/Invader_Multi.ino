#include <Arduboy2.h>
#include "src/utils/Constants.h"

void killGame() {

    DEBUG_PRINTLN("killGame()");

    controlState.setGameState(GameState::Title_Init);
    I2C::end();
    #ifdef I2C_USE_UNTILS
        role = I2C::Role::None;
    #else
        role = I2C::Role::Controller;
    #endif
    arduboy.pollButtons();

}

void onReceive() {

    targetState = *reinterpret_cast<const State *>(I2C::getBuffer());
    onReceive_Status = true;

}

void onRequest() {

    I2C::reply(controlState);
    onRequest_Status = true;
}

#ifdef I2C_USE_UNTILS
    I2C::CallbackAction exitMenu() {

            if (arduboy.pressed(B_BUTTON)) {
                return I2C::CallbackAction::Exit;
            }

            return I2C::CallbackAction::Continue;

    }
#else
    void exitMenu() {}
#endif

void waitForOther() {
    
    GameRotation gameRotation = controlState.getGameRotation();

    arduboy.clear();
    renderScenery(GameMode::Single, false);

    switch (gameRotation) {

        case GameRotation::Portrait:
            #ifndef DEBUG_LANDSCAPE
                Sprites::drawOverwrite(72, 5, Images::Portrait::WaitingForPlayer, 0);
                #ifdef I2C_USE_UNTILS
                    Sprites::drawOverwrite(56, 0, Images::Portrait::PressBToCancel, 0);
                #endif
            #endif
            break;

        case GameRotation::Landscape:
            #ifndef DEBUG_PORTRAIT
                Sprites::drawOverwrite(64 - 27, 4, Images::Landscape::WaitingForPlayer, 0);
                #ifdef I2C_USE_UNTILS
                    Sprites::drawOverwrite(64 - 32, 26, Images::Landscape::PressBToCancel, 0);
                #endif
            #endif
            break;

    }

    arduboy.display();

}

void flipCable() {
    
    GameRotation gameRotation = controlState.getGameRotation();

    arduboy.clear();
    renderScenery(GameMode::Single, false);

    switch (gameRotation) {

        case GameRotation::Portrait:
            #ifndef DEBUG_LANDSCAPE        
                Sprites::drawOverwrite(72, 12, Images::Portrait::FlipTheCable, 0);
                #ifdef I2C_USE_UNTILS
                    Sprites::drawOverwrite(56, 0, Images::Portrait::PressBToCancel, 0);
                #endif
            #endif
            break;

        case GameRotation::Landscape:
            #ifndef DEBUG_PORTRAIT
                Sprites::drawOverwrite(64 - 35, 4, Images::Landscape::FlipTheCable, 0);
                #ifdef I2C_USE_UNTILS
                    Sprites::drawOverwrite(64 - 32, 26, Images::Landscape::PressBToCancel, 0);
                #endif
            #endif
            break;

    }    

    arduboy.display();

}

void multi_Init() {

    controlState.setGameState(GameState::Multi);

}   

void multi() {

    GameRotation gameRotation = controlState.getGameRotation();

    #ifdef I2C_USE_UNTILS

        I2C::begin();
        I2C::CallbackOutcome result = I2C::checkCableFlippedUntil(flipCable, exitMenu);

        if (result == I2C::CallbackOutcome::Exited) {
            killGame();
            return;
        }

        role = I2C::handshakeUntil(waitForOther, exitMenu);

        if (role == I2C::Role::None) {
            killGame();
            return;
        }
        else if (role == I2C::Role::Target) {

            DEBUG_PRINTLN("I am the Target");
            I2C::onReceive(onReceive);
            I2C::onRequest(onRequest);
            onReceive_Status = true;

        }
        else {
            DEBUG_PRINTLN("I am the Controller");
        }

    #else
    
        I2C::begin();
        I2C::checkCableFlipped(flipCable, exitMenu);
        I2C::handshake(waitForOther, exitMenu);

        if (role == I2C::Role::Target) {

            DEBUG_PRINTLN("I am the Target");
            I2C::onReceive(onReceive);
            I2C::onRequest(onRequest);
            onReceive_Status = true;

        }
        else {
            DEBUG_PRINTLN("I am the Controller");
        }

    #endif

   
    switch (controlState.getGameMode()) {
    
        case GameMode::Single:
        case GameMode::Double:
            controlState.setGameState(GameState::Game_Init);
            break;
    
        case GameMode::TugOfWar:
            controlState.setGameState(GameState::TugOfWar_Init);
            break;
            
    }

}
