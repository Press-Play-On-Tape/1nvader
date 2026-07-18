#include <Arduboy2.h>

void gameOver_Init() {

    GameRotation gameRotation = controlState.getGameRotation();

    controlState.setGameState(GameState::GameOver);
    gameOverScreenVars.reset();

    switch (gameRotation) {

        case GameRotation::Portrait:
            #ifndef DEBUG_LANDSCAPE
                gameOverScreenVars.counter = 130;
            #endif
            break;

        case GameRotation::Landscape:
            #ifndef DEBUG_PORTRAIT
                gameOverScreenVars.counter = -20;
            #endif
            break;

    }

    uint16_t oldScore = EEPROM_Utils::getScore(controlState.getGameMode());
    uint16_t score = (controlState.getGameMode() == GameMode::Single ? controlPlayer.getScore() : controlPlayer.getScore() > targetPlayer.getScore() ? controlPlayer.getScore() : targetPlayer.getScore());

    if (score > 80) {

        if (EEPROM_Utils::getLevel(0) == 0) {

            EEPROM_Utils::saveLevel(0, 1);

        }
        else if (score > 160) {

            if (EEPROM_Utils::getLevel(1) == 0) {

                EEPROM_Utils::saveLevel(1, 1);
                
            }

        }

    }


    if (controlState.getGameMode() == GameMode::Double) {

        gameOverScreenVars.flashcontrolPlayer = controlPlayer.getScore() > targetPlayer.getScore();
        gameOverScreenVars.flashtargetPlayer = targetPlayer.getScore() > controlPlayer.getScore();
        
    }
    
    if (oldScore < score) {

        if (controlState.getGameMode() == GameMode::Single) {
            gameOverScreenVars.flashcontrolPlayer = true;
        }

        gameOverScreenVars.newHighScore = true;
        EEPROM_Utils::saveScore(controlState.getGameMode(), score);

    }

    #ifdef SOUNDS
        sound.tones(Sounds::Game_Over);
    #endif

}   


void gameOver() {

    GameRotation gameRotation = controlState.getGameRotation();

    // If multi player and we're the controller (master), ...

    if (controlState.getGameMode() != GameMode::Single) {
        
        if (role == I2C::Role::Controller) {

            I2C::read(I2C::targetAddress, targetState);
            readAddrNackError = 10;
            I2C::write(I2C::targetAddress, controlState, I2C::Mode::Async);

        }  
        else {

            onReceive_Status = false;

            controlState.setGameState(targetState.getGameState());
            controlState.setGameMode(targetState.getGameMode());
            controlState.setGameRotation(targetState.getGameRotation());
            controlState.mothership.clone(targetState.mothership);
            controlState.bomb.clone(targetState.bomb);
            controlState.gamePlayVars.clone(targetState.gamePlayVars);
            controlState.targetPlayer.clone(targetState.targetPlayer);
            controlState.controlPlayer.clone(targetState.controlPlayer);
            
        } 

    }

    // Move mothership ..

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                if (gameOverScreenVars.counter > 75) {

                    gameOverScreenVars.counter--;

                }
            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                if (gameOverScreenVars.counter < 4) {

                    gameOverScreenVars.counter++;

                }
            #endif

            break;

    }


    // Handle User Input -----------------------------------------------------

    if (arduboy.justPressed(A_BUTTON)) { 
        
        controlState.setGameState(GameState::Title_Init);

    }


    // Render screen ---------------------------------------------------------------------

    renderScores(gameOverScreenVars.flashcontrolPlayer, gameOverScreenVars.flashtargetPlayer);
    renderScenery(controlState.getGameMode() , true);

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                Sprites::drawExternalMask(gameOverScreenVars.counter, 11, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

                if (gameOverScreenVars.counter == 75) {

                    Sprites::drawSelfMasked(52, 17, Images::Portrait::GameOver, 0);
                    
                    if (!gameOverScreenVars.newHighScore || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                        renderHighScore(controlState.getGameMode(), false);
                    }

                }
            
            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT

                Sprites::drawExternalMask(43, gameOverScreenVars.counter, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

                if (gameOverScreenVars.counter == 4) {

                    Sprites::drawSelfMasked(32, 32, Images::Landscape::GameOver, 0);
                    
                    if (!gameOverScreenVars.newHighScore || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                        renderHighScore(controlState.getGameMode(), true);
                    }

                }

            #endif

            break;

    }

}
