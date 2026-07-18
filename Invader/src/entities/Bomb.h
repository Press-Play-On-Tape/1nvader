#pragma once

#include "../utils/Constants.h"

struct Bomb {

    private:

        int8_t pos = 28;
        int8_t height = 28;
        uint8_t explodingCount = 0;
        bool active = false;
        bool exploding = false;

    public:

        int8_t getPos()                                     { return this->pos; }
        int8_t getHeight()                                  { return this->height; }
        bool getActive()                                    { return this->active; }
        bool getExploding()                                 { return this->exploding; }
        uint8_t getExplosionCounter()                       { return this->explodingCount; }

        void setPos(int8_t val)                             { this->pos = val; }
        void setHeight(int8_t val)                          { this->height = val; }
        void setExploding(uint8_t val)                      { this->exploding = val; }
        void setActive(bool val)                            { this->active = val; this->exploding = false; }
        void setExplosionCounter(uint8_t val)               { this->explodingCount = val; }

    public:

        void clone(Bomb &bomb) {

            this->setPos(bomb.getPos());
            this->setHeight(bomb.getHeight());
            this->setExploding(bomb.getExploding());
            this->setActive(bomb.getActive());
            this->setExplosionCounter(bomb.getExplosionCounter());

        }

        void decHeight(GameRotation gameRotation) {

            switch (gameRotation) {

                case GameRotation::Portrait:

                    #ifndef DEBUG_LANDSCAPE
                        this->height--;
                        if (this->height < -7) {
                            this->active = false;
                        }
                    #endif
                    
                    break;

                case GameRotation::Landscape:

                    #ifndef DEBUG_PORTRAIT
                        this->height++;
                        if (this->height > 71) {
                            this->active = false;
                        }
                    #endif

                    break;

            }

            if (this->explodingCount > 0) {
                
                this->explodingCount--;

                if (this->explodingCount == 0) {
                    this->active = false;
                }

            }

        }

        void explode() {

            this->exploding = true;
            this->explodingCount = 4;

        }

};
