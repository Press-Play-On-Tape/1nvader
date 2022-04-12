#include "src/utils/Arduboy2Ext.h"

static constexpr uint8_t ppotFrames = (1.6 *60); // 1.6s animation loop


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void splashScreen_Init() {

    gameState = GameState::Splash;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen() { 

    auto justPressed = arduboy.justPressedButtons();

    if (justPressed > 0) {

        gameState = GameState::Title_Init; 

    }


    uint8_t p = 69; // Default pixel position 0 is hidden in the image

    switch (gameRotation) {

        case GameRotation::Portrait:

            Sprites::drawOverwrite(51, 0, Images::Portrait::PPOT, 0);

            switch (arduboy.getFrameCount(ppotFrames)) {

                // 2nd quarter of frames
                case (ppotFrames*1/4) ... (ppotFrames*2/4)-1:
                    p = 68; // Move pixel across to position 1
                    /*-fallthrough*/

                // 1st quarter of frames
                case 0 ... (ppotFrames*1/4)-1:
                    // Flash 'Play' arrow by clearing the image
                    // Overwrite using blank sprite 
                    Sprites::drawOverwrite(67, 58, Images::Portrait::PPOT_Blank, 0);
                    break;

                // 3rd quarter of frames
                case (ppotFrames*2/4) ... (ppotFrames*3/4)-1:
                    p = 67; // Move pixel across to position 2
                    break;

                // 4th quarter of frames
                default:
                    p = 66; // Move pixel across to position 3
                    break;

            }
            // Draw pixel to represent the tape spooling
            // Render using pixel function 
            arduboy.drawPixel(p, 20, WHITE);

            // On even steps of pixel movement, update the spindle image
            if (p % 2 == 0) {
                arduboy.sBuffer[68 +(1*WIDTH)] = 0x4B;
                arduboy.sBuffer[69 +(1*WIDTH)] = 0x2B;
            }

            break;

        default: // GameRotation::Landscape:

            Sprites::drawOverwrite(32, 16, Images::Landscape::PPOT, 0);

            p = 0x11; // Default pixel position 0 is hidden in the image

            switch (arduboy.getFrameCount(ppotFrames)) {

                // 2nd quarter of frames
                case (ppotFrames*1/4) ... (ppotFrames*2/4)-1:
                    p = 0x31; // Move pixel down to position 1
                    /*-fallthrough*/

                // 1st quarter of frames
                case 0 ... (ppotFrames*1/4)-1:
                    // Flash 'Play' arrow by clearing the image
                    // Overwrite directly to the screen buffer 
                    arduboy.sBuffer[91 +(3*WIDTH)] = 0x00;
                    arduboy.sBuffer[92 +(3*WIDTH)] = 0x00;
                    arduboy.sBuffer[93 +(3*WIDTH)] = 0x00;
                    arduboy.sBuffer[94 +(3*WIDTH)] = 0x00;
                    arduboy.sBuffer[95 +(3*WIDTH)] = 0x00;
                    break;

                // 3rd quarter of frames
                case (ppotFrames*2/4) ... (ppotFrames*3/4)-1:
                    p = 0x51; // Move pixel down to position 2
                    break;

                // 4th quarter of frames
                default:
                    p = 0x91; // Move pixel down to position 3
                    break;

            }

            // Draw pixel to represent the tape spooling
            // Render directly to the screen buffer 
            arduboy.sBuffer[52 +(3*WIDTH)] = p; // Values 0x11, 0x31, 0x51, 0x91

            // On even steps of pixel movement, update the spindle image
            if ((p & 0xA0) == 0) {
                // Render directly to the screen buffer 
                arduboy.sBuffer[45 +(3*WIDTH)] = 0xA5;
                arduboy.sBuffer[46 +(3*WIDTH)] = 0x95;
            }

            break;

    }

}
