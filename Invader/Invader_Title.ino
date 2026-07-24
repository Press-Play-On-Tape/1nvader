#include <Arduboy2.h>
#include "src/utils/Constants.h"

void title_Init() {

    GameRotation gameRotation = controlState.getGameRotation();

    controlState.setGameState(GameState::Title);
    titleScreenVars.reset(gameRotation);

    #ifdef SOUNDS
        sound.tones(Sounds::Theme);
    #endif

    titleScreenVars.levels = 2;

}   



void title() {

    GameRotation gameRotation = controlState.getGameRotation();

    renderScenery(GameMode::Single, true);
    renderHighScore(controlState.getGameMode(), false);
    
    if (arduboy.justPressed(B_BUTTON)) {

        gameRotation = gameRotation == GameRotation::Landscape ? GameRotation::Portrait : GameRotation::Landscape;
        controlState.setGameRotation(gameRotation);
        controlState.setGameState(GameState::Title_Init);
        EEPROM_Utils::saveRotation(gameRotation);

    }

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                Sprites::drawExternalMask(titleScreenVars.counter, 12, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

                switch (titleScreenVars.counter) {

                    case 89:
                        renderPlayerSelection(true);
                        renderInvaderSign();
                        break;

                    case 90 ... 97:
                        renderPlayerSelection(true);
                        renderInvaderSign();
                        titleScreenVars.counter++;
                        break;

                    case 98 ... 105:
                        renderPlayerSelection(false);
                        renderInvaderSign();
                        titleScreenVars.counter++;
                        break;

                    case 106 ... 128:
                        renderPlayerSelection(false);
                        titleScreenVars.counter++;
                        break;

                    case 129:

                        switch (controlState.getGameMode()) {

                            case GameMode::Single:
                                controlState.setGameState(GameState::Game_Init);
                                break;

                            case GameMode::Double:
                                controlState.setGameState(GameState::Multi_Init);
                                break;

                            case GameMode::TugOfWar:
                                controlState.setGameState(GameState::Multi_Init);
                                break;

                        }

                        break;
                        
                }

                if (arduboy.justPressed(RIGHT_BUTTON) && controlState.getGameMode() != GameMode::Single) { 

                    controlState.decGameMode();
                
                }

                if (arduboy.justPressed(LEFT_BUTTON) && static_cast<uint8_t>(controlState.getGameMode()) < titleScreenVars.levels) { 

                    controlState.incGameMode();

                }

                if (arduboy.justPressed(A_BUTTON) && titleScreenVars.counter == 89) { 

                    titleScreenVars.counter = 90;

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT

                Sprites::drawExternalMask(14, titleScreenVars.counter, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

                switch (titleScreenVars.counter) {

                    case 0:
                        renderPlayerSelection(true);
                        renderInvaderSign();
                        break;

                    case -8 ... -1:
                        renderPlayerSelection(true);
                        renderInvaderSign();
                        titleScreenVars.counter--;
                        break;

                    case -18 ... -9:
                        renderPlayerSelection(false);
                        renderInvaderSign();
                        titleScreenVars.counter--;
                        break;

                    case -38 ... -19:
                        renderPlayerSelection(false);
                        titleScreenVars.counter--;
                        break;

                    case -39:

                        switch (controlState.getGameMode()) {

                            case GameMode::Single:
                                controlState.setGameState(GameState::Game_Init);
                                break;

                            case GameMode::Double:
                                controlState.setGameState(GameState::Multi_Init);
                                break;

                            case GameMode::TugOfWar:

                                if (gameRotation == GameRotation::Landscape) {
                                    controlState.setGameRotation(GameRotation::Portrait);
                                    EEPROM_Utils::saveRotation(gameRotation);
                                }
                                
                                controlState.setGameState(GameState::Multi_Init);
                                break;

                        }

                        break;
                        
                }

                if (arduboy.justPressed(UP_BUTTON) && controlState.getGameMode() != GameMode::Single) { 

                    controlState.decGameMode();

                }


                if (arduboy.justPressed(DOWN_BUTTON) && static_cast<uint8_t>(controlState.getGameMode()) < titleScreenVars.levels) { 

                    controlState.incGameMode();

                }

                if (arduboy.justPressed(A_BUTTON) && titleScreenVars.counter == 0) { 

                    titleScreenVars.counter = -1;

                }

            #endif

            break;

    }

}

void renderPlayerSelection(bool renderPlayerSelection) {

    GameRotation gameRotation = controlState.getGameRotation();
    GameMode gameMode = controlState.getGameMode();
    
    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                Sprites::drawOverwrite(31, 20, Images::Portrait::Rotate, 0);

                if (!renderPlayerSelection) return;

                Sprites::drawSelfMasked(62 - (static_cast<uint8_t>(gameMode) * 8), 5, Images::Portrait::DownArrow, Constants::Arrow_Frames[arduboy.getFrameCount(48) / 12]);
                Sprites::drawOverwrite(62, 13, Images::Portrait::SurvivalMode, 0);
                Sprites::drawOverwrite(54, 13, Images::Portrait::VSMode, 0);
                Sprites::drawOverwrite(46, 13, Images::Portrait::TugOfWarMode, 0);

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                
                Sprites::drawOverwrite(85, 53, Images::Landscape::Rotate, 0);

                if (!renderPlayerSelection) return;

                Sprites::drawSelfMasked(71, 12 + (static_cast<uint8_t>(gameMode) * 9), Images::Landscape::LeftArrow, Constants::Arrow_Frames[arduboy.getFrameCount(48) / 12]);
                Sprites::drawOverwrite(79, 12, Images::Landscape::SurvivalMode, 0);
                Sprites::drawOverwrite(79, 21, Images::Landscape::VSMode, 0);
                Sprites::drawOverwrite(79, 30, Images::Landscape::TugOfWarMode, 0);

            #endif

            break;
            
    }

}
