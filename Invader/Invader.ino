#include "src/utils/Arduboy2Ext.h"
#define I2C_IMPLEMENTATION
#include <ArduboyI2C.h>

#include "src/entities/Entities.h"
#include "src/images/Images.h"
#include "src/utils/Constants.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/utils/Structs.h"
#include "src/sounds/Sounds.h"

#ifdef SOUNDS
#include <ArduboyTones.h>
#include "src/sounds/Sounds.h"
#endif

Arduboy2Ext arduboy;
// ARDUBOY_NO_USB

#ifdef SOUNDS
ArduboyTones sound(arduboy.audio.enabled);
#endif

TitleScreenVars titleScreenVars;
GameOverScreenVars gameOverScreenVars;
Particle particles[Constants::ParticlesMax];

State thisState;
State otherState;

Player &thisPlayer          = thisState.thisPlayer;
Player &otherPlayer         = thisState.otherPlayer;
Mothership &mothership      = thisState.mothership;
Bomb &bomb                  = thisState.bomb;
GamePlayVars &gamePlayVars  = thisState.gamePlayVars;

I2C::Role role;
volatile bool onReceive_Status = false;
volatile bool onRequest_Status = false;
uint8_t readAddrNackError = 10;

void setup() {
  
    arduboy.boot();
    arduboy.systemButtons();
    arduboy.setFrameRate(60);
    arduboy.initRandomSeed();
    arduboy.flashlight();
    arduboy.audio.begin();

    #ifdef SOUNDS
    arduboy.audio.begin();
    #endif

    EEPROM_Utils::initEEPROM(false);

    thisState.setGameState(GameState::Splash_Init);
    thisState.setGameMode(GameMode::Single);
    thisState.setGameRotation(GameRotation::Portrait);

}   


void loop() {

    if ( !arduboy.nextFrame() ) return;    
	arduboy.pollButtons();

    switch (thisState.getGameState()) {

        case GameState::Splash_Init:

            splashScreen_Init();
            splashScreen();
            break;

        case GameState::Splash:

            splashScreen();
            break;

        case GameState::Title_Init:

            title_Init();
            title();
            break;

        case GameState::Title:

            title();
            break;

        case GameState::Multi_Init:

            multi_Init();
            break;

        case GameState::Multi:

            multi();
            break;

        case GameState::Game_Init:

            game_Init();
            game();
            break;

        case GameState::Game:

            game();
            break;

        case GameState::TugOfWar_Init:

            tugOfWar_Init();
            tugOfWar();
            break;

        case GameState::TugOfWar:
            // arduboy.setCursor(56,0);
            // if (role == I2C::Role::Controller) {
            //     arduboy.print("L L");
            // }
            // else {
            //     arduboy.print("R R");
            // }
            tugOfWar();
            break;
            
        case GameState::GameOver_Init:

            gameOver_Init();
            gameOver();
            break;

        case GameState::GameOver:

            gameOver();
            break;

    }

    arduboy.display(true);

}

