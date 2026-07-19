#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Utils.h"
#include "src/utils/Constants.h"

void launchParticles(GameRotation gameRotation) {

    switch (gameRotation) {

        case GameRotation::Portrait:
            #ifndef DEBUG_LANDSCAPE
                launchParticles(gameRotation, 32, 64);
            #endif
            break;

        case GameRotation::Landscape:
            #ifndef DEBUG_PORTRAIT
                launchParticles(gameRotation, 64, 32);
            #endif
            break;

    }

}

void launchParticles(GameRotation gameRotation, int16_t x, int16_t y) {

    for (int i = 0; i < Constants::ParticlesMax; i++) {

        particles[i].setX(x);
        particles[i].setY(y);
        particles[i].setVelX(random(-4, 5));
        particles[i].setVelY(random(-1, 6));
        particles[i].setCounter(random(10, 46));
        particles[i].setSize(random(1, 3));
        particles[i].setRotation(gameRotation);
    
    }

}

void updateAndRenderParticles(GameRotation gameRotation, GameMode gameMode) {

    for ( int i = 0; i < Constants::ParticlesMax; i++) {

        particles[i].update();

        if ( particles[i].render() ) { // the dot should be rendered

            int pSize = particles[i].getSize();

            switch (gameRotation) {

                case GameRotation::Portrait:

                    #ifndef DEBUG_LANDSCAPE

                        if (gameMode == GameMode::TugOfWar) {

                            if (role == I2C::Role::Controller) {
                                
                                if (pSize == 1) {
                                    arduboy.drawPixel(particles[i].getY(), particles[i].getX(), 1);
                                } 
                                else {
                                    arduboy.drawRect(particles[i].getY(), particles[i].getX(), pSize, pSize, 1);
                                }

                            }
                            else {
                                
                                if (pSize == 1) {
                                    arduboy.drawPixel(particles[i].getX(), particles[i].getY(), 1);
                                } 
                                else {
                                    arduboy.drawRect(particles[i].getX(), particles[i].getY(), pSize, pSize, 1);
                                }

                            }
                        
                        }
                        else {

                            if (pSize == 1) {
                                arduboy.drawPixel(particles[i].getY(), particles[i].getX(), 1);
                            } 
                            else {
                                arduboy.drawRect(particles[i].getY(), particles[i].getX(), pSize, pSize, 1);
                            }

                        }

                    #endif

                    break;

                case GameRotation::Landscape:

                    #ifndef DEBUG_PORTRAIT

                        if (pSize == 1) {
                            arduboy.drawPixel( particles[i].getX(), particles[i].getY(), 1);
                        } 
                        else {
                            arduboy.drawRect( particles[i].getX(), particles[i].getY(), pSize, pSize, 1);
                        }

                    #endif

                    break;
                    
            }

        }

    }

}
