#include <Arduboy2.h>

void tugOfWar_Init() {

    gameState = GameState::TugOfWar;
    readAddrNackError = 10;

    mothership.reset(gameRotation, 0);
    mothership.setHeight(60);

    if (role == I2C::Role::Controller) {

        thisPlayer.reset(0);
        thisPlayer.setPos(45);
        thisPlayer.setMovement(Movement::Up);

        otherPlayer.reset(1);
        otherPlayer.setPos(25);
        otherPlayer.setMovement(Movement::Down);

    }
    else {
    
        thisPlayer.reset(1);
        thisPlayer.setPos(25);
        thisPlayer.setMovement(Movement::Down);

        otherPlayer.reset(0);
        otherPlayer.setPos(45);
        otherPlayer.setMovement(Movement::Up);

    }

    arduboy.frameCount = 0;

}   

void tugOfWar() {

    if (arduboy.justPressed(B_BUTTON)) { 
        DEBUG_PRINTLN("tugOfWar() -> killGame(A)");
        killGame();
        return;
    }

    // If we're the controller (master), ...
   
    if (role == I2C::Role::Controller) {

        I2C::read(I2C::targetAddress, otherPlayer);

        if (I2C::getError() == I2C::Error::ReadAddrNack) {

            if (readAddrNackError > 0) {
                readAddrNackError--;
            }
            else {
                DEBUG_PRINTLN("tugOfWar() -> killGame(B)");
                killGame();
                return;
            }

        }
        
        I2C::write(I2C::targetAddress, thisPlayer, I2C::Mode::Async);

    }  
    else {

        if (!onReceive_Status) {
            DEBUG_PRINTLN("tugOfWar() -> killGame(C)");
            killGame();
            return;
        }

        onReceive_Status = false;
    } 


    // Handle movements ..

    // if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
    if (arduboy.justPressed(A_BUTTON) ) {

        bool fired = thisPlayer.fire(gameRotation, gameMode, nullptr);

        #ifdef SOUNDS
            if (fired) sound.tones(Sounds::Player_Fires_Bullet);
        #endif

    }

    // if ((arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))) {

    //      bool fired = otherPlayer.fire(gameRotation, gameMode, nullptr);

    //     #ifdef SOUNDS
    //         if (fired) sound.tones(Sounds::Player_Fires_Bullet);
    //     #endif

    // }
    
    movePlayer(thisPlayer, otherPlayer);
    movePlayer(otherPlayer, thisPlayer);
    mothership.moveTugOfWar(thisPlayer, otherPlayer);

    if (thisPlayer.getBulletActive())      moveBullet(thisPlayer); 
    if (otherPlayer.getBulletActive())     moveBullet(otherPlayer);



    // End of game?

    if ((mothership.getHeight() <= Constants::PlayerHeight) ||
        (mothership.getHeight() >= 124 - Constants::PlayerHeight)) {

        switch (mothership.getMovement()) {

            case Movement::Up:
                if (mothership.getPosDisplay() < -Constants::MothershipHeight) {
                    gameState = GameState::GameOver_Init;
                }
                break;

            case Movement::Down:
                if (mothership.getPosDisplay() > HEIGHT) {
                    gameState = GameState::GameOver_Init;
                }
                break;

            default: break;

        }

    }
       

    renderScenery(gameMode, true);
    renderScoreTugOfWar(otherPlayer.getScore(), thisPlayer.getScore());

    // if (role == I2C::Role::Controller) {
    //     arduboy.setCursor(0,0);
    //     arduboy.print("This:");
    //     arduboy.print(thisPlayer.getPos());
    //     arduboy.print(" Other:");
    //     arduboy.print(otherPlayer.getPos());
    // }
    // else {
    //     arduboy.setCursor(0,0);
    //     arduboy.print("This:");
    //     arduboy.print(thisPlayer.getPos());
    //     arduboy.print(" Other:");
    //     arduboy.print(otherPlayer.getPos());
    // }

    updateAndRenderParticles(GameRotation::Landscape);

    Sprites::drawExternalMask(0, thisPlayer.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

    if (thisPlayer.getExplosionCounter() > 0) {

        Sprites::drawSelfMasked(0, thisPlayer.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - thisPlayer.getExplosionCounter()) / 2);

    }

    Sprites::drawExternalMask(120, otherPlayer.getPos(), Images::Portrait::Rotated::Player, Images::Portrait::Rotated::Player_Mask, 0, 0);

    if (otherPlayer.getExplosionCounter() > 0) {

        Sprites::drawSelfMasked(120, otherPlayer.getPos() - 4, Images::Portrait::Rotated::Player_Explosion, (6 - otherPlayer.getExplosionCounter()) / 2);

    }

    if (role == I2C::Role::Controller) {

        switch (mothership.getExplosionCounter()) {

            case 0:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                break;

            case 1 ... Constants::MothershipExplosionMax / 2:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                break;

            case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax - 1:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                break;

            case Constants::MothershipExplosionMax:
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                launchParticles(GameRotation::Landscape, mothership.getHeight() + (Constants::MothershipWidth / 2), mothership.getPosDisplay() + (Constants::MothershipHeight / 2));
                break;

        }

        if (thisPlayer.getBulletActive())   Sprites::drawExternalMask(thisPlayer.getBulletX(), thisPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
        if (otherPlayer.getBulletActive())  Sprites::drawExternalMask(otherPlayer.getBulletX(), 64 - 7 - otherPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);

    }
    else {

        switch (mothership.getExplosionCounter()) {

            case 0:
                Sprites::drawExternalMask(120 - mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                break;

            case 1 ... Constants::MothershipExplosionMax / 2:
                Sprites::drawExternalMask(120 - mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                break;

            case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax - 1:
                Sprites::drawExternalMask(120 - mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(120 - mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                break;

            case Constants::MothershipExplosionMax:
                Sprites::drawExternalMask(120 - mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                Sprites::drawExternalMask(120 - mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                launchParticles(GameRotation::Landscape, mothership.getHeight() + (Constants::MothershipWidth / 2), mothership.getPosDisplay() + (Constants::MothershipHeight / 2));
                break;

        }

        if (thisPlayer.getBulletActive())  Sprites::drawExternalMask(120 - thisPlayer.getBulletX(), thisPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
        if (otherPlayer.getBulletActive())  Sprites::drawExternalMask(120 - otherPlayer.getBulletX(), otherPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);

    }

}
