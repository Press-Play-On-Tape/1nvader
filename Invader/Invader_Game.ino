#include <Arduboy2.h>

void game_Init() {

    GameRotation gameRotation = controlState.getGameRotation();

    controlState.setGameState(GameState::Game);
    readAddrNackError = 10;

    mothership.reset(gameRotation, gameRotation == GameRotation::Landscape ? -Constants::MothershipRowHeight : Constants::MothershipRowHeight);
    gamePlayVars.bombCounter = random(256, 1024);

    controlPlayer.reset(0);
    targetPlayer.reset(1);

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                switch (controlState.getGameMode()) {

                    case GameMode::Single:
                        controlPlayer.setPos(26);
                        controlPlayer.setMovement(Movement::Up);
                        break;

                    default:

                        if (role == I2C::Role::Controller) {

                            controlPlayer.setPlayerIdx(0);
                            controlPlayer.setPos(13);
                            controlPlayer.setMovement(Movement::Up);

                            targetPlayer.setPlayerIdx(1);
                            targetPlayer.setPos(38);
                            targetPlayer.setMovement(Movement::Down);

                        }
                        else {

                            controlPlayer.setPlayerIdx(1);
                            controlPlayer.setPos(38);
                            controlPlayer.setMovement(Movement::Down);

                            targetPlayer.setPlayerIdx(0);
                            targetPlayer.setPos(13);
                            targetPlayer.setMovement(Movement::Up);

                        }

                        break;

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                
                switch (controlState.getGameMode()) {

                    case GameMode::Single:
                        controlPlayer.setPos(59);
                        controlPlayer.setMovement(Movement::Left);
                        break;

                    default:

                        if (role == I2C::Role::Controller) {
                            
                            controlPlayer.setPlayerIdx(0);
                            controlPlayer.setPos(33);
                            controlPlayer.setMovement(Movement::Left);

                            targetPlayer.setPlayerIdx(1);
                            targetPlayer.setPos(84);
                            targetPlayer.setMovement(Movement::Right);

                        } 
                        else {
                            
                            controlPlayer.setPlayerIdx(1);
                            controlPlayer.setPos(84);
                            controlPlayer.setMovement(Movement::Right);

                            targetPlayer.setPlayerIdx(0);
                            targetPlayer.setPos(33);
                            targetPlayer.setMovement(Movement::Left);

                        }                        
                        
                        break;

                }

            #endif
                
            break;


    }            

}   

void game() {

    GameRotation gameRotation = controlState.getGameRotation();

    if (arduboy.justPressed(B_BUTTON)) { 
        DEBUG_PRINTLN("game() -> killGame(A)");
        killGame();
        return;
    }


    // If multi player and we're the controller (master), ...

    if (controlState.getGameMode() != GameMode::Single) {
        
        if (role == I2C::Role::Controller) {

            I2C::read(I2C::targetAddress, targetState);

            // if (I2C::getError() == I2C::Error::ReadAddrNack) {

            //     if (readAddrNackError > 0) {
            //         readAddrNackError--;
            //     }
            //     else {
            //         DEBUG_PRINTLN("game() -> killGame(B)");
            //         killGame();
            //         return;
            //     }

            // }

            readAddrNackError = 10;
            I2C::write(I2C::targetAddress, controlState, I2C::Mode::Sync);

        }  
        else {

            // if (!onReceive_Status) {
            //     DEBUG_PRINTLN("game() -> killGame(C)");
            //     killGame();
            // }

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


    // Randomly drop a bomb ?

    if (!gamePlayVars.waveCleared) {

        if (role == I2C::Role::Controller) {
            
            if (controlPlayer.getScore() + targetPlayer.getScore() >= 20) {

                if ((gameRotation == GameRotation::Landscape && mothership.getHeight() < 30) || (gameRotation == GameRotation::Portrait && mothership.getHeight() > 56)) {

                    gamePlayVars.bombCounter--;

                    if (gamePlayVars.bombCounter == 0) {

                        #ifdef SOUNDS
                            sound.tones(Sounds::Enemy_Drops_Bomb);
                        #endif

                        bomb.setActive(true);
                        bomb.setPos(mothership.getPosDisplay() + 6);
                        bomb.setHeight(mothership.getHeight() + 6);

                        gamePlayVars.bombCounter = random(256, 1024);

                    }

                }

            }

        }


        // Handle movements ..

        GameMode gameMode = controlState.getGameMode();
        targetPlayer.setJustPressed(0);

        if (!controlPlayer.getBeingPushed()) {

            if (controlState.getGameMode() == GameMode::Double) {

                if (arduboy.justPressed(A_BUTTON)) {

                    if (role == I2C::Role::Controller) {    

                        bool fired = controlPlayer.fire(gameRotation, gameMode, &targetPlayer);

                        #ifdef SOUNDS
                            if (fired) sound.tones(Sounds::Player_Fires_Bullet);
                        #endif

                    }
                    else {

                        GameMode gameMode = controlState.getGameMode();
                        bool fired = targetPlayer.fire(gameRotation, gameMode, &controlPlayer);
                        targetPlayer.setJustPressed(arduboy.justPressedButtons());

                        #ifdef SOUNDS
                            if (fired) sound.tones(Sounds::Player_Fires_Bullet);
                        #endif
                    
                    }

                }

                if (role == I2C::Role::Controller) {    

                    if (targetState.targetPlayer.getJustPressed() & A_BUTTON) {

                        bool fired = targetPlayer.fire(gameRotation, gameMode, &controlPlayer);

                    }

                }
                
            }
            else {
                
                if (arduboy.justPressed(A_BUTTON)) {

                    GameMode gameMode = controlState.getGameMode();
                    bool fired = controlPlayer.fire(gameRotation, gameMode, nullptr);

                    #ifdef SOUNDS
                        if (fired) sound.tones(Sounds::Player_Fires_Bullet);
                    #endif

                }

            }

        }

        if (controlState.getGameMode() == GameMode::Double) {

            if (role == I2C::Role::Controller) {

                movePlayer(controlPlayer, targetPlayer);
                movePlayer(targetPlayer, controlPlayer);
                mothership.move(gameRotation, controlState.getGameMode(), controlPlayer, targetPlayer);

                if (controlPlayer.getBulletActive() && !gamePlayVars.waveCleared)     moveBullet(controlPlayer); 
                if (targetPlayer.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(targetPlayer);
                if (bomb.getActive()) moveBomb();

                controlPlayer.decExplodeCounter();
                targetPlayer.decExplodeCounter();

            }
            else {

                if (controlPlayer.getBulletActive() && !gamePlayVars.waveCleared)     moveBullet(controlPlayer); 
                if (targetPlayer.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(targetPlayer);

            }
            
        }
        else {

            moveControlPlayer();
            mothership.move(gameRotation, controlState.getGameMode(), controlPlayer);

            if (controlPlayer.getBulletActive() && !gamePlayVars.waveCleared)     moveBullet(controlPlayer); 
            if (targetPlayer.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(targetPlayer);
            if (bomb.getActive()) moveBomb();

            controlPlayer.decExplodeCounter();
            targetPlayer.decExplodeCounter();

        }



        // End of game?

        switch (gameRotation) {

            case GameRotation::Portrait:

                #ifndef DEBUG_LANDSCAPE

                    if (mothership.getHeight() < Constants::PlayerHeight) {

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

                #endif

                break;
            
            case GameRotation::Landscape:

                #ifndef DEBUG_PORTRAIT

                    if (mothership.getHeight() > 64 - Constants::PlayerHeight - Constants::MothershipHeight) {

                        switch (mothership.getMovement()) {

                            case Movement::Left:
                                if (mothership.getPosDisplay() < -Constants::MothershipHeight) {
                                    controlState.setGameState(GameState::GameOver_Init);
                                }
                                break;

                            case Movement::Right:
                                if (mothership.getPosDisplay() > WIDTH) {
                                    controlState.setGameState(GameState::GameOver_Init);
                                }
                                break;

                            default: break;

                        }

                    }

                #endif

                break;


        }    

    }    



    // Render screen ---------------------------------------------------------------------

    renderScores(false, false);
    renderScenery(controlState.getGameMode(), true);
    updateAndRenderParticles(gameRotation, controlState.getGameMode());

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                Sprites::drawExternalMask(0, controlPlayer.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

                if (controlPlayer.getExplosionCounter() > 0) {

                    Sprites::drawSelfMasked(0, controlPlayer.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - controlPlayer.getExplosionCounter()) / 2);

                }

                if (controlState.getGameMode() == GameMode::Double) {

                    Sprites::drawExternalMask(0, targetPlayer.getPos(), Images::Portrait::Normal::Player2, Images::Portrait::Normal::Player2_Mask, 0, 0);

                    if (targetPlayer.getExplosionCounter() > 0) {

                        Sprites::drawSelfMasked(0, targetPlayer.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - targetPlayer.getExplosionCounter()) / 2);

                    }

                }

                switch (mothership.getExplosionCounter()) {

                    case 0:
                        Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                        break;

                    case 1 ... Constants::MothershipExplosionMax / 2:
                        Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                        break;

                    case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax:
                        Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Normal::Mothership, Images::Portrait::Normal::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                        Sprites::drawExternalMask(mothership.getHeight(), mothership.getPosDisplay(), Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                        break;

                }

                if (controlPlayer.getBulletActive())  Sprites::drawExternalMask(controlPlayer.getBulletX(), controlPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
                if (targetPlayer.getBulletActive())  Sprites::drawExternalMask(targetPlayer.getBulletX(), targetPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
                
                if (bomb.getActive()) {

                    if (bomb.getExplosionCounter() == 0) {

                        Sprites::drawExternalMask(bomb.getHeight(), bomb.getPos(), Images::Portrait::Bomb, Images::Portrait::Bomb_Mask, 0, 0);
                        
                    }

                    switch (bomb.getHeight()) {

                        case -2 ... -1:
                            Sprites::drawExternalMask(0, bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                            break;

                        case -4 ... -3:
                            Sprites::drawExternalMask(0, bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                            break;

                    }        

                    if (bomb.getExploding()) {

                        switch (bomb.getExplosionCounter()) {

                            case 1 ... 2:
                                Sprites::drawExternalMask(bomb.getHeight(), bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 1, 1);
                                break;

                            case 3 ... 4:
                                Sprites::drawExternalMask(bomb.getHeight(), bomb.getPos() - 4, Images::Portrait::Explosion, Images::Portrait::Explosion_Mask, 0, 0);
                                break;

                        }        

                    }

                }    

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT

                Sprites::drawExternalMask(controlPlayer.getPos(), 56, Images::Landscape::Player, Images::Landscape::Player_Mask, 0, 0);

                if (controlPlayer.getExplosionCounter() > 0) {

                    Sprites::drawSelfMasked(controlPlayer.getPos() - 4, 52, Images::Landscape::Player_Explosion, (6 - controlPlayer.getExplosionCounter()) / 2);

                }

                if (controlState.getGameMode() == GameMode::Double) {

                    Sprites::drawExternalMask(targetPlayer.getPos(), 56, Images::Landscape::Player2, Images::Landscape::Player2_Mask, 0, 0);

                    if (targetPlayer.getExplosionCounter() > 0) {

                        Sprites::drawSelfMasked(targetPlayer.getPos() - 4, 52, Images::Landscape::Player_Explosion, (6 - targetPlayer.getExplosionCounter()) / 2);

                    }

                }

                switch (mothership.getExplosionCounter()) {

                    case 0:
                        Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Mothership, Images::Landscape::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                        break;

                    case 1 ... Constants::MothershipExplosionMax / 2:
                        Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 1, 1);
                        break;

                    case (Constants::MothershipExplosionMax / 2) + 1 ... Constants::MothershipExplosionMax:
                        Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Mothership, Images::Landscape::Mothership_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);
                        Sprites::drawExternalMask(mothership.getPosDisplay(), mothership.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 0, 0);
                        break;

                }

                if (controlPlayer.getBulletActive())  Sprites::drawExternalMask(controlPlayer.getBulletX(), controlPlayer.getBulletY(), Images::Landscape::Laser, Images::Landscape::Laser_Mask, 0, 0);
                if (targetPlayer.getBulletActive())  Sprites::drawExternalMask(targetPlayer.getBulletX(), targetPlayer.getBulletY(), Images::Landscape::Laser, Images::Landscape::Laser_Mask, 0, 0);
                
                if (bomb.getActive()) {

                    if (bomb.getExplosionCounter() == 0) {

                        Sprites::drawExternalMask(bomb.getPos(), bomb.getHeight(), Images::Landscape::Bomb, Images::Landscape::Bomb_Mask, 0, 0);

                    }

                    switch (bomb.getHeight()) {

                        case 60 ... 62:
                            Sprites::drawExternalMask(bomb.getPos() - 4, 58, Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 0, 0);
                            break;

                        case 63 ... 64:
                            Sprites::drawExternalMask(bomb.getPos() - 4, 58, Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 1, 1);
                            break;

                    }        

                    if (bomb.getExploding()) {

                        switch (bomb.getExplosionCounter()) {

                            case 1 ... 2:
                                Sprites::drawExternalMask(bomb.getPos() - 4, bomb.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 1, 1);
                                break;

                            case 3 ... 4:
                                Sprites::drawExternalMask(bomb.getPos() - 4, bomb.getHeight(), Images::Landscape::Explosion, Images::Landscape::Explosion_Mask, 0, 0);
                                break;

                        }        

                    }

                }        

            #endif

            break;

    }

    if (gamePlayVars.waveCleared) {


        // Clear an bullets from the screen ..

        if (controlPlayer.getBulletActive()) controlPlayer.setBulletActive(false);
        if (targetPlayer.getBulletActive()) targetPlayer.setBulletActive(false);

        if (gamePlayVars.waveCounter == 8) {
            #ifdef SOUNDS
                sound.tones(Sounds::Wave_Cleared);
            #endif
        }

        if (gamePlayVars.waveCounter >= 55 && arduboy.isFrameCount(64)) { launchParticles(gameRotation); }

        if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {

            gamePlayVars.waveCounter = 0;
            gamePlayVars.waveCleared = false;

        }

        if (controlState.getGameMode() == GameMode::Double) {

            movePlayer(controlPlayer, targetPlayer);
            movePlayer(targetPlayer, controlPlayer);


            if (mothership.getExplosionCounter() > 0) mothership.move(gameRotation, controlState.getGameMode(), controlPlayer, targetPlayer);

        }
        else {

            moveControlPlayer();
            if (mothership.getExplosionCounter() > 0) mothership.move(gameRotation, controlState.getGameMode(), controlPlayer);

        }

        uint8_t idx = gamePlayVars.waveCounter / 8;

        updateAndRenderParticles(gameRotation, controlState.getGameMode());

        switch (gameRotation) {

            case GameRotation::Portrait:

                #ifndef DEBUG_LANDSCAPE
                    arduboy.drawRect(60, 32 - (idx * 4), 9, 4 + (idx * 8), BLACK);
                    Sprites::drawSelfMasked(60, 32 - (idx * 4), Images::Portrait::WaveCleared[idx], 0);
                #endif

                break;

            case GameRotation::Landscape:

                #ifndef DEBUG_PORTRAIT
                    arduboy.drawRect(60 - (idx * 4), 24, 4 + (idx * 8), 9, BLACK);
                    Sprites::drawSelfMasked(60 - (idx * 4), 24, Images::Landscape::WaveCleared[idx], 0);
                #endif

                break;

        }         

        if (gamePlayVars.waveCounter < 55) gamePlayVars.waveCounter++;

    }
// DEBUG_BREAK
}


void movePlayer(Player &player, Player &targetPlayer) {

    GameRotation gameRotation = controlState.getGameRotation();

    if (arduboy.isFrameCount(2, player.getPlayerIdx())) {

        switch (player.getMovement()) {

            case Movement::Up:

                if (player.getPos() > Constants::Portrait::PlayerMinPos) {

                    player.decPos();
                    
                    if (controlState.getGameMode() == GameMode::TugOfWar) return;

                    if (abs(player.getPos() - targetPlayer.getPos()) < Constants::PlayerWidthNoMask ) {
                        
                        if (targetPlayer.getPos() > Constants::Portrait::PlayerMinPos && targetPlayer.getMovement() == Movement::Down) {
                            targetPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.incPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;

            case Movement::Down:

                if (player.getPos() < Constants::Portrait::PlayerMaxPos) {

                    player.incPos();
                    
                    if (controlState.getGameMode() == GameMode::TugOfWar) return;

                    if (abs(player.getPos() - targetPlayer.getPos()) < Constants::PlayerWidthNoMask) {

                        if (targetPlayer.getPos() < Constants::Portrait::PlayerMaxPos && targetPlayer.getMovement() == Movement::Up) {
                            targetPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.decPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;

            case Movement::Left:

                if (player.getPos() > Constants::Landscape::PlayerMinPos) {

                    player.decPos();

                    if (abs(player.getPos() - targetPlayer.getPos()) < Constants::PlayerWidthNoMask ) {
                        
                        if (targetPlayer.getPos() > Constants::Landscape::PlayerMinPos && targetPlayer.getMovement() == Movement::Right) {
                            targetPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.incPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;

            case Movement::Right:

                if (player.getPos() < Constants::Landscape::PlayerMaxPos) {

                    player.incPos();

                    if (abs(player.getPos() - targetPlayer.getPos()) < Constants::PlayerWidthNoMask) {

                        if (targetPlayer.getPos() < Constants::Landscape::PlayerMaxPos && targetPlayer.getMovement() == Movement::Left) {
                            targetPlayer.changeMovement(gameRotation);
                        }
                        else {
                            player.decPos();
                            player.changeMovement(gameRotation);
                        }

                    }

                }
                else {
                    player.changeMovement(gameRotation);
                }

                break;


        }

    }

}


void moveControlPlayer() {

    GameRotation gameRotation = controlState.getGameRotation();

    if (arduboy.isFrameCount(2, 0)) {

        switch (controlPlayer.getMovement()) {

            case Movement::Up:
                controlPlayer.decPos();
                if (controlPlayer.getPos() == Constants::Portrait::PlayerMinPos) {
                    controlPlayer.changeMovement(gameRotation);
                }
                break;

            case Movement::Left:
                controlPlayer.decPos();
                if (controlPlayer.getPos() == Constants::Landscape::PlayerMinPos) {
                    controlPlayer.changeMovement(gameRotation);
                }
                break;

            case Movement::Down:
                controlPlayer.incPos();
                if (controlPlayer.getPos() == Constants::Portrait::PlayerMaxPos) {
                    controlPlayer.changeMovement(gameRotation);
                }
                break;

            case Movement::Right:
                controlPlayer.incPos();
                if (controlPlayer.getPos() == Constants::Landscape::PlayerMaxPos) {
                    controlPlayer.changeMovement(gameRotation);
                }
                break;

        }

    }

}

void moveBullet(Player &player) {

    GameRotation gameRotation = controlState.getGameRotation();

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                switch (controlState.getGameMode()) {
                
                    case GameMode::Single ... GameMode::Double:
                        player.incBulletX();
                        break;
                
                    case GameMode::TugOfWar:

                        switch (player.getPlayerIdx()) {

                            case 0:
                                player.incBulletX();
                                break;

                            case 1:
                                player.decBulletX();
                                break;

                        }
                        break;

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                if (arduboy.isFrameCount(2, 0)) {
                    player.decBulletY();
                }
            #endif

            break;

    }

    if (mothership.getExplosionCounter() != 0) return;
    if (!player.getBulletActive()) return;


    switch (gameRotation) {

        case GameRotation::Portrait:
            {

                #ifndef DEBUG_LANDSCAPE
                    
                    Rect bulletRect = { player.getBulletX() + 1, player.getBulletY(), Constants::BulletHeight - 2, Constants::BulletWidth };
                    Rect mothershipRect = { mothership.getHeight() + 1, mothership.getPosDisplay() + 1, Constants::MothershipHeight - 2, Constants::MothershipWidth - 2 };

                    if (arduboy.collide(bulletRect, mothershipRect)) {

                        #ifdef SOUNDS
                            sound.tones(Sounds::Enemy_Explosion);
                        #endif


                        launchParticles(gameRotation, mothership.getPosDisplay() + (Constants::MothershipHeight / 2), mothership.getHeight() + (Constants::MothershipWidth / 2));

                        if (controlState.getGameMode() == GameMode::TugOfWar) {
                            mothership.explode(player.getPlayerIdx() == 0 ? -Constants::TugOfWarRowAdjustment : Constants::TugOfWarRowAdjustment);
                            gamePlayVars.waveCleared = false;
                            mothership.decCounter();
                        }
                        else {
                            mothership.explode(Constants::MothershipRowHeight);
                            gamePlayVars.waveCleared = mothership.decCounter();
                        }

                        player.incScore();
                        player.setBulletActive(false);

                    }

                    if (bomb.getActive()) {

                        Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombHeight - 2, Constants::BombWidth - 2 };

                        if (arduboy.collide(bulletRect, bombRect)) {

                            #ifdef SOUNDS
                                sound.tones(Sounds::Bomb_Explosion);
                            #endif

                            bomb.explode();
                            player.setBulletActive(false);

                        }
                        
                    }

                #endif

            }

            break;

        case GameRotation::Landscape:
            {    

                #ifndef DEBUG_PORTRAIT    
                    Rect bulletRect = { player.getBulletX() + 1, player.getBulletY() + 1, Constants::BulletWidth - 2, Constants::BulletHeight - 2 };
                    Rect mothershipRect = { mothership.getPosDisplay() + 1, mothership.getHeight() + 1, Constants::MothershipWidth - 2, Constants::MothershipHeight - 2 };

                    if (arduboy.collide(bulletRect, mothershipRect)) {

                        #ifdef SOUNDS
                            sound.tones(Sounds::Enemy_Explosion);
                        #endif

                        launchParticles(gameRotation, mothership.getPosDisplay() + (Constants::MothershipHeight / 2), mothership.getHeight() + (Constants::MothershipWidth / 2));

                        mothership.explode(-Constants::MothershipRowHeight);                    
                        gamePlayVars.waveCleared = mothership.decCounter();

                        player.incScore();
                        player.setBulletActive(false);

                    }

                    if (bomb.getActive()) {

                        Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombWidth - 2, Constants::BombHeight - 2 };

                        if (arduboy.collide(bulletRect, bombRect)) {

                            #ifdef SOUNDS
                                sound.tones(Sounds::Bomb_Explosion);
                            #endif

                            bomb.explode();
                            player.setBulletActive(false);

                        }
                        
                    }

                #endif

            }

            break;

    }

}

void moveBomb() {

    GameRotation gameRotation = controlState.getGameRotation();
    bomb.decHeight(gameRotation);

    if (!bomb.getActive() || bomb.getExplosionCounter() > 0) return;

    switch (gameRotation) {

        case GameRotation::Portrait:
            {

                #ifndef DEBUG_LANDSCAPE

                    Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombHeight - 2, Constants::BombWidth - 2 };
                    Rect controlPlayerRect = { 0, controlPlayer.getPos() + 1, Constants::PlayerHeight - 2, Constants::PlayerWidth - 2 };

                    if (arduboy.collide(bombRect, controlPlayerRect)) {

                        bool explode = controlPlayer.explode();

                        #ifdef SOUNDS
                            if (explode) {
                                sound.tones(Sounds::Player_Hit_By_Bomb);
                            }
                        #endif
                    }

                    if (controlState.getGameMode() != GameMode::Single) {

                        Rect targetPlayerRect = { 0, targetPlayer.getPos() + 1, Constants::PlayerHeight - 2, Constants::PlayerWidth - 2 };

                        if (arduboy.collide(bombRect, targetPlayerRect)) {

                            bool explode = targetPlayer.explode();

                            #ifdef SOUNDS
                                if (explode) {
                                    sound.tones(Sounds::Player_Hit_By_Bomb);
                                }
                            #endif

                        }

                    }

                #endif

            }

            break;

        case GameRotation::Landscape:
            {      

                #ifndef DEBUG_PORTRAIT  
                    
                    Rect bombRect = { bomb.getPos() + 1, bomb.getHeight() + 1, Constants::BombWidth - 2, Constants::BombHeight - 2 };
                    Rect controlPlayerRect = { controlPlayer.getPos() + 1, 64 - Constants::PlayerHeight + 1, Constants::PlayerWidth - 2, Constants::PlayerHeight - 1 };

                    if (arduboy.collide(bombRect, controlPlayerRect)) {

                        bool explode = controlPlayer.explode();

                        #ifdef SOUNDS
                            if (explode) {
                                sound.tones(Sounds::Player_Hit_By_Bomb);
                            }
                        #endif

                    }

                    if (controlState.getGameMode() != GameMode::Single) {

                        Rect targetPlayerRect = { targetPlayer.getPos() + 1, 64 - Constants::PlayerHeight + 1, Constants::PlayerWidth - 2, Constants::PlayerHeight - 1 };

                        if (arduboy.collide(bombRect, targetPlayerRect)) {

                            bool explode = targetPlayer.explode();

                            #ifdef SOUNDS
                                if (explode) {
                                    sound.tones(Sounds::Player_Hit_By_Bomb);
                                }
                            #endif

                        }

                    }

                #endif

            }

            break;

    }

}
