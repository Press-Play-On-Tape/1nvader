#include <Arduboy2.h>

void tugOfWar_Init() {

    GameRotation gameRotation = controlState.getGameRotation();

    controlState.setGameState(GameState::TugOfWar);
    readAddrNackError = 10;

    mothership.reset(gameRotation, 0);
    mothership.setHeight(60);

    if (role == I2C::Role::Controller) {

        controlPlayer.reset(0);
        controlPlayer.setPos(45);
        controlPlayer.setMovement(Movement::Up);

        targetPlayer.reset(1);
        targetPlayer.setPos(25);
        targetPlayer.setMovement(Movement::Down);

    }
    else {
    
        controlPlayer.reset(1);
        controlPlayer.setPos(25);
        controlPlayer.setMovement(Movement::Down);

        targetPlayer.reset(0);
        targetPlayer.setPos(45);
        targetPlayer.setMovement(Movement::Up);

    }

    arduboy.frameCount = 0;

}   

void tugOfWar() {

    GameRotation gameRotation = controlState.getGameRotation();

    if (arduboy.justPressed(B_BUTTON)) { 
        DEBUG_PRINTLN("tugOfWar() -> killGame(A)");
        killGame();
        return;
    }

    // If we're the controller (master), ...
   
    if (role == I2C::Role::Controller) {

        I2C::read(I2C::targetAddress, targetPlayer);

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
        
        readAddrNackError = 10;
        I2C::write(I2C::targetAddress, controlPlayer, I2C::Mode::Async);

    }  
    else {

        // uint8_t x = 0;
        // I2C::setAddress(I2C::targetAddress);
        // // wait for the controller (master) to send us its input
        // while (!onReceive_Status) { 
        // DEBUG_BREAK
        //     x++;
        //     if (x == 255) {
        //     DEBUG_PRINTLN("tugOfWar() -> killGame(C)");
        //     killGame();
        //     return;            
        //     }
        // }
        // // store the controller's input and reset the flag
        // // rightInput = controllerInput;
        // onReceive_Status = false;

        // // wait for the controller (master) to request our input
        // while (!onRequest_Status) { }
        // // reset the flag
        // onRequest_Status = false;
        // // set our address to the null address; we're done
        // // otherwise the controller may get ahead of us and request our input again before we have a chance to update it
        // // thus destroying our synchronization
        // I2C::setAddress(I2C::nullAddress);

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

        bool fired = controlPlayer.fire(gameRotation, controlState.getGameMode(), nullptr);

        #ifdef SOUNDS
            if (fired) sound.tones(Sounds::Player_Fires_Bullet);
        #endif

    }

    // if ((arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))) {

    //      bool fired = targetPlayer.fire(gameRotation, gameMode, nullptr);

    //     #ifdef SOUNDS
    //         if (fired) sound.tones(Sounds::Player_Fires_Bullet);
    //     #endif

    // }
    
    movePlayer(controlPlayer, targetPlayer);
    movePlayer(targetPlayer, controlPlayer);
    mothership.moveTugOfWar(controlPlayer, targetPlayer);

    if (controlPlayer.getBulletActive())      moveBullet(controlPlayer); 
    if (targetPlayer.getBulletActive())     moveBullet(targetPlayer);



    // End of game?

    if ((mothership.getHeight() <= Constants::PlayerHeight) ||
        (mothership.getHeight() >= 124 - Constants::PlayerHeight)) {

        switch (mothership.getMovement()) {

            case Movement::Up:
                if (mothership.getPosDisplay() < -Constants::MothershipHeight) {
                    controlState.setGameState(GameState::GameOver_Init);
                }
                break;

            case Movement::Down:
                if (mothership.getPosDisplay() > HEIGHT) {
                    controlState.setGameState(GameState::GameOver_Init);
                }
                break;

            default: break;

        }

    }
       

    renderScenery(controlState.getGameMode() , true);
    renderScoreTugOfWar(targetPlayer.getScore(), controlPlayer.getScore());

    // if (role == I2C::Role::Controller) {
    //     arduboy.setCursor(0,0);
    //     arduboy.print("This:");
    //     arduboy.print(controlPlayer.getPos());
    //     arduboy.print(" Other:");
    //     arduboy.print(targetPlayer.getPos());
    // }
    // else {
    //     arduboy.setCursor(0,0);
    //     arduboy.print("This:");
    //     arduboy.print(controlPlayer.getPos());
    //     arduboy.print(" Other:");
    //     arduboy.print(targetPlayer.getPos());
    // }

    updateAndRenderParticles(GameRotation::Landscape);

    Sprites::drawExternalMask(0, controlPlayer.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

    if (controlPlayer.getExplosionCounter() > 0) {

        Sprites::drawSelfMasked(0, controlPlayer.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - controlPlayer.getExplosionCounter()) / 2);

    }

    Sprites::drawExternalMask(120, targetPlayer.getPos(), Images::Portrait::Rotated::Player, Images::Portrait::Rotated::Player_Mask, 0, 0);

    if (targetPlayer.getExplosionCounter() > 0) {

        Sprites::drawSelfMasked(120, targetPlayer.getPos() - 4, Images::Portrait::Rotated::Player_Explosion, (6 - targetPlayer.getExplosionCounter()) / 2);

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

        if (controlPlayer.getBulletActive())   Sprites::drawExternalMask(controlPlayer.getBulletX(), controlPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
        if (targetPlayer.getBulletActive())  Sprites::drawExternalMask(targetPlayer.getBulletX(), 64 - 7 - targetPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);

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

        if (controlPlayer.getBulletActive())  Sprites::drawExternalMask(120 - controlPlayer.getBulletX(), controlPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
        if (targetPlayer.getBulletActive())  Sprites::drawExternalMask(120 - targetPlayer.getBulletX(), targetPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);

    }

}
