#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Utils.h"

void renderScores(bool flashthisPlayer, bool flashotherPlayer) {

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                switch (gameMode) {
                    
                    case GameMode::Single:

                        if (!flashthisPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                            renderScore(16, 121, thisPlayer.getScore());
                        }

                        Sprites::drawOverwrite(121, 41, Images::Portrait::Normal::Numbers, mothership.getCounter() / 10);
                        Sprites::drawOverwrite(121, 45, Images::Portrait::Normal::Numbers, mothership.getCounter() % 10);

                        break;

                    case GameMode::Double:

                        if (!flashthisPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                            renderScore(3, 121, thisPlayer.getScore());
                        }

                        if (!flashotherPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                            renderScore(46, 121, otherPlayer.getScore());
                        }

                        Sprites::drawOverwrite(121, 28, Images::Portrait::Normal::Numbers, mothership.getCounter() / 10);
                        Sprites::drawOverwrite(121, 32, Images::Portrait::Normal::Numbers, mothership.getCounter() % 10);

                        break;
                    
                    case GameMode::TugOfWar:

                        if (!flashthisPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                            renderScore(3, 116, thisPlayer.getScore());
                        }

                        if (!flashotherPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                            renderScore(46, 116, otherPlayer.getScore());
                        }

                        break;

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT                
                
                if (gameMode == GameMode::Double) {

                    if (!flashthisPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                        renderScore(36, 0, thisPlayer.getScore());
                    }

                    if (!flashotherPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                        renderScore(100, 0, otherPlayer.getScore());
                    }

                    Sprites::drawOverwrite(61, 0, Images::Landscape::Numbers, mothership.getCounter() / 10);
                    Sprites::drawOverwrite(65, 0, Images::Landscape::Numbers, mothership.getCounter() % 10);

                }
                else {

                    if (!flashthisPlayer || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                        renderScore(36, 0, thisPlayer.getScore());
                    }

                    Sprites::drawOverwrite(91, 0, Images::Landscape::Numbers, mothership.getCounter() / 10);
                    Sprites::drawOverwrite(95, 0, Images::Landscape::Numbers, mothership.getCounter() % 10);

                }

            #endif

            break;

    }

}

void renderScore(uint8_t location, uint8_t height, uint16_t score) {

    uint8_t digits[4] = {};
    extractDigits(digits, score);

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE

                for (uint8_t j = 4; j > 0; --j, location += 4) {

                    Sprites::drawOverwrite(height, location, Images::Portrait::Normal::Numbers, digits[j - 1]);

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                
                for (uint8_t j = 0; j < 4; ++j, location -= 4) {

                    Sprites::drawOverwrite(location, height, Images::Landscape::Numbers, digits[j]);

                }
            
            #endif

            break;

    }

}


void renderScoreTugOfWar(uint16_t score1, uint16_t score2) {

    uint8_t digits[4] = {};
    extractDigits(digits, score2);


    uint8_t location = 1;
    for (uint8_t j = 4; j > 0; --j, location += 4) {

        Sprites::drawSelfMasked(113, location, Images::Portrait::Normal::Numbers, digits[j - 1]);

    }

    extractDigits(digits, score1);
    location = 48;
    for (uint8_t j = 4; j > 0; --j, location += 4) {

        Sprites::drawSelfMasked(113, location, Images::Portrait::Normal::Numbers, digits[j - 1]);

    }

}

void renderSceneryPortrait(bool renderStars) {

    if (renderStars) {
        
        if (arduboy.isFrameCount(2, 0)) {

            arduboy.drawPixel(36, 4, WHITE);
            arduboy.drawPixel(61, 25, WHITE);

        }
        else {

            arduboy.drawPixel(45, 52, WHITE);
            arduboy.drawPixel(106, 54, WHITE);
            arduboy.drawPixel(96, 24, WHITE);

        }

    }

    for (int8_t y = -2; y < 64; y+=16) {

        Sprites::drawOverwrite(17, y, Images::Portrait::Mountain, 0);

    }

    Sprites::drawOverwrite(0, 0, Images::Portrait::Ground, 0);

}

void renderScenery(GameMode gameMode, bool renderStars) {

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                switch (gameMode) {

                    case GameMode::TugOfWar:

                        renderSceneryPortrait(renderStars);

                        for (int8_t y = -2; y < 64; y+=16) {

                            Sprites::drawOverwrite(103, y, Images::Portrait::Mountain_Top, 0);

                        }

                        Sprites::drawOverwrite(125, 0, Images::Portrait::Ground_Top, 0);

                        for (uint8_t y = 1; y < 64; y = y + 4) {

                            arduboy.drawFastVLine(64, y, 2);

                        } 

                        break;

                    default:

                        renderSceneryPortrait(renderStars);
                        break;

                }

            #endif

            break;

        case GameRotation::Landscape:
                
            #ifndef DEBUG_PORTRAIT 

                if (renderStars) {
                    
                    if (arduboy.isFrameCount(2, 0)) {

                        arduboy.drawPixel(6, 4, WHITE);
                        arduboy.drawPixel(61, 25, WHITE);
                        arduboy.drawPixel(102, 38, WHITE);
                        arduboy.drawPixel(126, 37, WHITE);

                    }
                    else {

                        arduboy.drawPixel(10, 22, WHITE);
                        arduboy.drawPixel(75, 3, WHITE);
                        arduboy.drawPixel(124, 6, WHITE);

                    }

                }

                for (int16_t x = -2; x < 127; x+=16) {

                    Sprites::drawOverwrite(x, 42, Images::Landscape::Mountain, 0);

                }

                for (uint8_t x = 0; x < 128; x+=8) {

                    Sprites::drawOverwrite(x, 61, Images::Landscape::Ground, 0);

                }

            #endif

            break;

    }
}

void renderHighScore(GameMode gameMode, bool centre) {

    uint8_t digits[4] = {};

    uint16_t score = EEPROM_Utils::getScore(gameMode);
    extractDigits(digits, score);

    switch (gameRotation) {

        case GameRotation::Portrait:
            {
                #ifndef DEBUG_LANDSCAPE
                    
                    uint8_t y = 45;

                    Sprites::drawOverwrite(7, 5, Images::Portrait::HighScore, 0);

                    for (uint8_t j = 4; j > 0; --j, y += 4) {
                        Sprites::drawOverwrite(7, y, Images::Portrait::Normal::Numbers, digits[j - 1]);
                    }

                #endif

            }

            break;

        case GameRotation::Landscape:
            {
                #ifndef DEBUG_PORTRAIT
                    
                    uint8_t x = centre ? 77 : 51;

                    Sprites::drawSelfMasked(centre ? 32 : 6, 53, Images::Landscape::HighScore, 0);

                    for (uint8_t j = 0; j < 4; ++j, x += 4) {
                        Sprites::drawSelfMasked(x, 53, Images::Landscape::Numbers, digits[3 - j]);
                    }

                #endif

            }

            break;

    }

}

void renderInvaderSign() {

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                Sprites::drawOverwrite(78, 7, Images::Portrait::Invader, 0);
            #endif
            
            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                Sprites::drawSelfMasked(9, 31, Images::Landscape::Invader, 0);
            #endif

            break;
    
    }

}

