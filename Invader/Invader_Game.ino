#include <Arduboy2.h>

void game_Init() {

    gameState = GameState::Game;
    readAddrNackError = 10;

    mothership.reset(gameRotation, gameRotation == GameRotation::Landscape ? -Constants::MothershipRowHeight : Constants::MothershipRowHeight);
    gamePlayVars.bombCounter = random(256, 1024);

    thisPlayer.reset(0);
    otherPlayer.reset(1);

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                switch (gameMode) {

                    case GameMode::Single:
                        thisPlayer.setPos(26);
                        thisPlayer.setMovement(Movement::Up);
                        break;

                    default:

                        if (role == I2C::Role::Controller) {

                            thisPlayer.setPlayerIdx(0);
                            thisPlayer.setPos(13);
                            thisPlayer.setMovement(Movement::Up);

                            otherPlayer.setPlayerIdx(1);
                            otherPlayer.setPos(38);
                            otherPlayer.setMovement(Movement::Down);

                        }
                        else {

                            thisPlayer.setPlayerIdx(1);
                            thisPlayer.setPos(38);
                            thisPlayer.setMovement(Movement::Down);

                            otherPlayer.setPlayerIdx(0);
                            otherPlayer.setPos(13);
                            otherPlayer.setMovement(Movement::Up);

                        }

                        break;

                }

            #endif

            break;

        case GameRotation::Landscape:

            #ifndef DEBUG_PORTRAIT
                
                switch (gameMode) {

                    case GameMode::Single:
                        thisPlayer.setPos(59);
                        thisPlayer.setMovement(Movement::Left);
                        break;

                    default:

                        if (role == I2C::Role::Controller) {
                            
                            thisPlayer.setPlayerIdx(0);
                            thisPlayer.setPos(33);
                            thisPlayer.setMovement(Movement::Left);

                            otherPlayer.setPlayerIdx(1);
                            otherPlayer.setPos(84);
                            otherPlayer.setMovement(Movement::Right);

                        } 
                        else {
                            
                            thisPlayer.setPlayerIdx(1);
                            thisPlayer.setPos(84);
                            thisPlayer.setMovement(Movement::Right);

                            otherPlayer.setPlayerIdx(0);
                            otherPlayer.setPos(33);
                            otherPlayer.setMovement(Movement::Left);

                        }                        
                        
                        break;

                }

            #endif
                
            break;


    }            

}   

void game() {

    if (arduboy.justPressed(B_BUTTON)) { 
        DEBUG_PRINTLN("game() -> killGame(A)");
        killGame();
        return;
    }


    // if we're the controller (master), ...

    if (gameMode != GameMode::Single) {
        
        if (role == I2C::Role::Controller) {

            I2C::read(I2C::targetAddress, otherPlayer);

            if (I2C::getError() == I2C::Error::ReadAddrNack) {

                if (readAddrNackError > 0) {
                    readAddrNackError--;
                }
                else {
                    DEBUG_PRINTLN("game() -> killGame(B)");
                    killGame();
                    return;
                }
            }

            I2C::write(I2C::targetAddress, thisPlayer, I2C::Mode::Async);

        }  
        else {

            if (!onReceive_Status) {
                DEBUG_PRINTLN("game() -> killGame(C)");
                killGame();
            }

            onReceive_Status = false;
        } 

    }


    // Randomly drop a bomb ?

    if (!gamePlayVars.waveCleared) {

        if (thisPlayer.getScore() + otherPlayer.getScore() > 20) {

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


        // Handle movements ..

        if (!thisPlayer.getBeingPushed()) {

            if (arduboy.justPressed(A_BUTTON)) {
                bool fired = thisPlayer.fire(gameRotation, gameMode, (gameMode == GameMode::Double ? &otherPlayer : nullptr));

                #ifdef SOUNDS
                    if (fired) sound.tones(Sounds::Player_Fires_Bullet);
                #endif

            }

        }

        if (gameMode == GameMode::Double) {

            movePlayer(thisPlayer, otherPlayer);
            movePlayer(otherPlayer, thisPlayer);
            mothership.move(gameRotation, gameMode, thisPlayer, otherPlayer);

        }
        else {

            movethisPlayer();
            mothership.move(gameRotation, gameMode, thisPlayer);

        }

        if (thisPlayer.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(thisPlayer); 
        if (otherPlayer.getBulletActive() && !gamePlayVars.waveCleared)      moveBullet(otherPlayer);
        if (bomb.getActive()) moveBomb();

        thisPlayer.decExplodeCounter();
        otherPlayer.decExplodeCounter();


        // End of game?

        switch (gameRotation) {

            case GameRotation::Portrait:

                #ifndef DEBUG_LANDSCAPE

                    if (mothership.getHeight() < Constants::PlayerHeight) {

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

                #endif

                break;
            
            case GameRotation::Landscape:

                #ifndef DEBUG_PORTRAIT

                    if (mothership.getHeight() > 64 - Constants::PlayerHeight - Constants::MothershipHeight) {

                        switch (mothership.getMovement()) {

                            case Movement::Left:
                                if (mothership.getPosDisplay() < -Constants::MothershipHeight) {
                                    gameState = GameState::GameOver_Init;
                                }
                                break;

                            case Movement::Right:
                                if (mothership.getPosDisplay() > WIDTH) {
                                    gameState = GameState::GameOver_Init;
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
    renderScenery(gameMode, true);
    updateAndRenderParticles(gameRotation);

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                Sprites::drawExternalMask(0, thisPlayer.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

                if (thisPlayer.getExplosionCounter() > 0) {

                    Sprites::drawSelfMasked(0, thisPlayer.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - thisPlayer.getExplosionCounter()) / 2);

                }

                if (gameMode == GameMode::Double) {

                    Sprites::drawExternalMask(0, otherPlayer.getPos(), Images::Portrait::Normal::Player, Images::Portrait::Normal::Player_Mask, 0, 0);

                    if (otherPlayer.getExplosionCounter() > 0) {

                        Sprites::drawSelfMasked(0, otherPlayer.getPos() - 4, Images::Portrait::Normal::Player_Explosion, (6 - otherPlayer.getExplosionCounter()) / 2);

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

                if (thisPlayer.getBulletActive())  Sprites::drawExternalMask(thisPlayer.getBulletX(), thisPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
                if (otherPlayer.getBulletActive())  Sprites::drawExternalMask(otherPlayer.getBulletX(), otherPlayer.getBulletY(), Images::Portrait::Laser, Images::Portrait::Laser_Mask, 0, 0);
                
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

                Sprites::drawExternalMask(thisPlayer.getPos(), 56, Images::Landscape::Player, Images::Landscape::Player_Mask, 0, 0);

                if (thisPlayer.getExplosionCounter() > 0) {

                    Sprites::drawSelfMasked(thisPlayer.getPos() - 4, 52, Images::Landscape::Player_Explosion, (6 - thisPlayer.getExplosionCounter()) / 2);

                }

                if (gameMode == GameMode::Double) {

                    Sprites::drawExternalMask(otherPlayer.getPos(), 56, Images::Landscape::Player, Images::Landscape::Player_Mask, 0, 0);

                    if (otherPlayer.getExplosionCounter() > 0) {

                        Sprites::drawSelfMasked(otherPlayer.getPos() - 4, 52, Images::Landscape::Player_Explosion, (6 - otherPlayer.getExplosionCounter()) / 2);

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

                if (thisPlayer.getBulletActive())  Sprites::drawExternalMask(thisPlayer.getBulletX(), thisPlayer.getBulletY(), Images::Landscape::Laser, Images::Landscape::Laser_Mask, 0, 0);
                if (otherPlayer.getBulletActive())  Sprites::drawExternalMask(otherPlayer.getBulletX(), otherPlayer.getBulletY(), Images::Landscape::Laser, Images::Landscape::Laser_Mask, 0, 0);
                
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

        if (thisPlayer.getBulletActive()) thisPlayer.setBulletActive(false);
        if (otherPlayer.getBulletActive()) otherPlayer.setBulletActive(false);

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

        if (gameMode == GameMode::Double) {

            movePlayer(thisPlayer, otherPlayer);
            movePlayer(otherPlayer, thisPlayer);


            if (mothership.getExplosionCounter() > 0) mothership.move(gameRotation, gameMode, thisPlayer, otherPlayer);

        }
        else {

            movethisPlayer();
            if (mothership.getExplosionCounter() > 0) mothership.move(gameRotation, gameMode, thisPlayer);

        }

        uint8_t idx = gamePlayVars.waveCounter / 8;

        updateAndRenderParticles(gameRotation);

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

}


void movePlayer(Player &player, Player &otherPlayer) {

    if (arduboy.isFrameCount(2, player.getPlayerIdx())) {

        switch (player.getMovement()) {

            case Movement::Up:

                if (player.getPos() > Constants::Portrait::PlayerMinPos) {

                    player.decPos();
                    
                    if (gameMode == GameMode::TugOfWar) return;

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask ) {
                        
                        if (otherPlayer.getPos() > Constants::Portrait::PlayerMinPos && otherPlayer.getMovement() == Movement::Down) {
                            otherPlayer.changeMovement(gameRotation);
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
                    
                    if (gameMode == GameMode::TugOfWar) return;

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask) {

                        if (otherPlayer.getPos() < Constants::Portrait::PlayerMaxPos && otherPlayer.getMovement() == Movement::Up) {
                            otherPlayer.changeMovement(gameRotation);
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

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask ) {
                        
                        if (otherPlayer.getPos() > Constants::Landscape::PlayerMinPos && otherPlayer.getMovement() == Movement::Right) {
                            otherPlayer.changeMovement(gameRotation);
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

                    if (abs(player.getPos() - otherPlayer.getPos()) < Constants::PlayerWidthNoMask) {

                        if (otherPlayer.getPos() < Constants::Landscape::PlayerMaxPos && otherPlayer.getMovement() == Movement::Left) {
                            otherPlayer.changeMovement(gameRotation);
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


void movethisPlayer() {

    if (arduboy.isFrameCount(2, 0)) {

        switch (thisPlayer.getMovement()) {

            case Movement::Up:
                thisPlayer.decPos();
                if (thisPlayer.getPos() == Constants::Portrait::PlayerMinPos) {
                    thisPlayer.changeMovement(gameRotation);
                }
                break;

            case Movement::Left:
                thisPlayer.decPos();
                if (thisPlayer.getPos() == Constants::Landscape::PlayerMinPos) {
                    thisPlayer.changeMovement(gameRotation);
                }
                break;

            case Movement::Down:
                thisPlayer.incPos();
                if (thisPlayer.getPos() == Constants::Portrait::PlayerMaxPos) {
                    thisPlayer.changeMovement(gameRotation);
                }
                break;

            case Movement::Right:
                thisPlayer.incPos();
                if (thisPlayer.getPos() == Constants::Landscape::PlayerMaxPos) {
                    thisPlayer.changeMovement(gameRotation);
                }
                break;

        }

    }

}

void moveBullet(Player &player) {

    switch (gameRotation) {

        case GameRotation::Portrait:

            #ifndef DEBUG_LANDSCAPE
                
                switch (gameMode) {
                
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
                    
                    Rect bulletRect = { player.getBulletX() + 1, player.getBulletY() +1, Constants::BulletHeight - 2, Constants::BulletWidth - 2 };
                    Rect mothershipRect = { mothership.getHeight() + 1, mothership.getPosDisplay() + 1, Constants::MothershipWidth - 2, Constants::MothershipHeight - 2 };

                    if (arduboy.collide(bulletRect, mothershipRect)) {

                        #ifdef SOUNDS
                            sound.tones(Sounds::Enemy_Explosion);
                        #endif


                        launchParticles(gameRotation, mothership.getPosDisplay() + (Constants::MothershipHeight / 2), mothership.getHeight() + (Constants::MothershipWidth / 2));


                        if (gameMode == GameMode::TugOfWar) {
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

    bomb.decHeight(gameRotation);

    if (!bomb.getActive() || bomb.getExplosionCounter() > 0) return;

    switch (gameRotation) {

        case GameRotation::Portrait:
            {

                #ifndef DEBUG_LANDSCAPE

                    Rect bombRect = { bomb.getHeight() + 1, bomb.getPos() + 1, Constants::BombHeight - 2, Constants::BombWidth - 2 };
                    Rect thisPlayerRect = { 0, thisPlayer.getPos() + 1, Constants::PlayerHeight - 2, Constants::PlayerWidth - 2 };

                    if (arduboy.collide(bombRect, thisPlayerRect)) {

                        bool explode = thisPlayer.explode();

                        #ifdef SOUNDS
                            if (explode) {
                                sound.tones(Sounds::Player_Hit_By_Bomb);
                            }
                        #endif
                    }

                    if (gameMode != GameMode::Single) {

                        Rect otherPlayerRect = { 0, otherPlayer.getPos() + 1, Constants::PlayerHeight - 2, Constants::PlayerWidth - 2 };

                        if (arduboy.collide(bombRect, otherPlayerRect)) {

                            bool explode = otherPlayer.explode();

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
                    Rect thisPlayerRect = { thisPlayer.getPos() + 1, 64 - Constants::PlayerHeight + 1, Constants::PlayerWidth - 2, Constants::PlayerHeight - 1 };

                    if (arduboy.collide(bombRect, thisPlayerRect)) {

                        bool explode = thisPlayer.explode();

                        #ifdef SOUNDS
                            if (explode) {
                                sound.tones(Sounds::Player_Hit_By_Bomb);
                            }
                        #endif

                    }

                    if (gameMode != GameMode::Single) {

                        Rect otherPlayerRect = { otherPlayer.getPos() + 1, 64 - Constants::PlayerHeight + 1, Constants::PlayerWidth - 2, Constants::PlayerHeight - 1 };

                        if (arduboy.collide(bombRect, otherPlayerRect)) {

                            bool explode = otherPlayer.explode();

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
