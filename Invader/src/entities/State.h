#pragma once

#include "../utils/Constants.h"
#include "Player.h"
#include "Mothership.h"
#include "Bomb.h"
#include "../utils/Structs.h"

struct State {

    private:
        GameState gameState;
        GameMode gameMode;
        GameRotation gameRotation;

    public:
        GamePlayVars gamePlayVars;
        Player thisPlayer;
        Player otherPlayer;
        Mothership mothership;
        Bomb bomb;

    public:

        GameState getGameState()                            { return this->gameState; }
        GameMode getGameMode()                              { return this->gameMode; }
        GameRotation getGameRotation()                      { return this->gameRotation; }

        void setGameMode(GameMode val)                      { this->gameMode = val; }
        void setGameState(GameState val)                    { this->gameState = val; }
        void setGameRotation(GameRotation val)              { this->gameRotation = val; }

        void incGameState() {
        
            this->gameState = static_cast<GameState>(static_cast<uint8_t>(this->gameState) + 1);

        }

        void decGameState() {
        
            this->gameState = static_cast<GameState>(static_cast<uint8_t>(this->gameState) - 1);

        }

        void incGameMode() {
        
            this->gameMode = static_cast<GameMode>(static_cast<uint8_t>(this->gameMode) + 1);

        }

        void decGameMode() {
        
            this->gameMode = static_cast<GameMode>(static_cast<uint8_t>(this->gameMode) - 1);

        }

};