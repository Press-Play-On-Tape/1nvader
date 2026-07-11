#pragma once

#include "Constants.h"

struct GamePlayVars {

    uint16_t bombCounter;
    uint8_t waveCounter;
    bool waveCleared;

};

struct TitleScreenVars {

    int16_t counter = 0;
    uint8_t levels = 0;

    void reset(GameRotation gameRotation) {
        
        switch (gameRotation) {

            case GameRotation::Portrait:
            
                this->counter = 89;
                break;

            case GameRotation::Landscape:
            
                this->counter = 0;
                break;

        }

    }

};

struct GameOverScreenVars {

    int16_t counter = 0;
    bool newHighScore = false;
    bool flashthisPlayer = false;
    bool flashotherPlayer = false;

    void reset() {

        this->counter = 128;
        newHighScore = false;
        flashthisPlayer = false;
        flashotherPlayer = false;

    }

};