#include <Arduboy2.h>


void killGame() {

    gameState = GameState::Title_Init; 
    I2C::end();
    role = I2C::Role::Controller;

}

void drawMessage(const __FlashStringHelper *message) {
    arduboy.clear();
    arduboy.print(message);
    arduboy.display();
}

void onReceive() {
    otherPlayer = *reinterpret_cast<const Player *>(I2C::getBuffer());
    onReceive_Status = true;
}

void onRequest() {
    I2C::reply(thisPlayer);
}

void exitMenu() {
    
    if (arduboy.pressed(A_BUTTON)) {
       killGame(); 
    }

}

void waitForOther() {
    
    arduboy.clear();
    renderScenery(GameMode::Single, false);

    switch (gameRotation) {

        case GameRotation::Portrait:
            #ifndef DEBUG_LANDSCAPE
                Sprites::drawOverwrite(66, 5, Images::Portrait::WaitingForPlayer, 0);
            #endif
            break;

        case GameRotation::Landscape:
            #ifndef DEBUG_PORTRAIT
                Sprites::drawOverwrite(64 - 27, 5, Images::Landscape::WaitingForPlayer, 0);
            #endif
            break;

    }

    arduboy.display();

}

void flipCable() {
    
    arduboy.clear();
    renderScenery(GameMode::Single, false);

    switch (gameRotation) {

        case GameRotation::Portrait:
            #ifndef DEBUG_LANDSCAPE        
                Sprites::drawOverwrite(39, 12, Images::Portrait::FlipTheCable, 0);
            #endif
            break;

        case GameRotation::Landscape:
            #ifndef DEBUG_PORTRAIT
                Sprites::drawOverwrite(39, 12, Images::Landscape::FlipTheCable, 0);
            #endif
            break;

    }    

    arduboy.display();

}

void multi_Init() {

    gameState = GameState::Multi;

}   


void multi() {


    
    // if (arduboy.justPressed(B_BUTTON)) {

    //     gameRotation = gameRotation == GameRotation::Landscape ? GameRotation::Portrait : GameRotation::Landscape;
    //     gameState = GameState::Title_Init;
    //     EEPROM_Utils::saveRotation(gameRotation);

    // }

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE

                I2C::begin();
                I2C::checkCableFlipped(flipCable);
                role = I2C::handshake(waitForOther, exitMenu);

                // if we're the target (slave), set up the receive and request callbacks
                if (role == I2C::Role::Target) {

                    // Serial.println("I am the Target.");
                    I2C::onReceive(onReceive);
                    I2C::onRequest(onRequest);

                }
                else {

                    // Serial.println("I am the Controller.");

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT

                I2C::begin();
                I2C::checkCableFlipped(flipCable);
                role = I2C::handshake(waitForOther, exitMenu);

                // if we're the target (slave), set up the receive and request callbacks
                if (role == I2C::Role::Target) {

                    // Serial.println("I am the Target.");
                    I2C::onReceive(onReceive);
                    I2C::onRequest(onRequest);

                }
                else {

                    // Serial.println("I am the Controller.");

                }

            #endif

            break;
            
    }

    
    switch (gameMode) {
    
        case GameMode::Single:
        case GameMode::Double:
            gameState = GameState::Game_Init;
            break;
    
        case GameMode::TugOfWar:
            gameState = GameState::TugOfWar_Init;
            break;
            
    }

}
