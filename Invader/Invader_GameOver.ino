#include <Arduboy2.h>

void gameOver_Init() {

    killGame();

    gameState = GameState::GameOver;
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

    uint16_t oldScore = EEPROM_Utils::getScore(gameMode);
    uint16_t score = (gameMode == GameMode::Single ? thisPlayer.getScore() : thisPlayer.getScore() > otherPlayer.getScore() ? thisPlayer.getScore() : otherPlayer.getScore());

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


    if (gameMode == GameMode::Double) {

        gameOverScreenVars.flashthisPlayer = thisPlayer.getScore() > otherPlayer.getScore();
        gameOverScreenVars.flashotherPlayer = otherPlayer.getScore() > thisPlayer.getScore();
        
    }
    
    if (oldScore < score) {

        if (gameMode == GameMode::Single) {
            gameOverScreenVars.flashthisPlayer = true;
        }

        gameOverScreenVars.newHighScore = true;
        EEPROM_Utils::saveScore(gameMode, score);

    }

    #ifdef SOUNDS
        sound.tones(Sounds::Game_Over);
    #endif

}   


void gameOver() {


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

        gameState = GameState::Title_Init;

    }


    // Render screen ---------------------------------------------------------------------

    renderScores(gameOverScreenVars.flashthisPlayer, gameOverScreenVars.flashotherPlayer);
    renderScenery(gameMode, true);

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                Sprites::drawExternalMask(gameOverScreenVars.counter, 11, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

                if (gameOverScreenVars.counter == 75) {

                    Sprites::drawSelfMasked(52, 17, Images::Portrait::GameOver, 0);
                    
                    if (!gameOverScreenVars.newHighScore || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                        renderHighScore(gameMode, false);
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
                        renderHighScore(gameMode, true);
                    }

                }

            #endif

            break;

    }

}
