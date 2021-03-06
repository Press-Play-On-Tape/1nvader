#pragma once

namespace Images {

    namespace Portrait {
       
        const uint8_t PROGMEM Laser[] = {
        7, 5,
        0x00, 0x04, 0x08, 0x04, 0x02, 0x04, 0x00, 
        };

        const uint8_t PROGMEM Laser_Mask[] = {
        0x04, 0x0e, 0x1c, 0x0e, 0x07, 0x0e, 0x04, 
        };
        
        namespace Normal {
            
            const uint8_t PROGMEM Player[] = {
            8, 11,
            0x54, 0xf8, 0x8e, 0xdc, 0x74, 0x20, 0x20, 0x00, 
            0x01, 0x00, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 
            };

            const uint8_t PROGMEM Player_Mask[] = {
            0xfe, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x70, 0x70, 
            0x03, 0x07, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 
            };

            const uint8_t PROGMEM Player_Explosion[] = {
            12, 19,
            0x00, 0x10, 0x38, 0x10, 0x00, 0x40, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x20, 0x70, 0x20, 0x00, 0x08, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

            0x00, 0x00, 0x08, 0x1c, 0x08, 0x00, 0x00, 0x40, 0xe0, 0x40, 0x00, 0x00, 
            0x00, 0x00, 0x80, 0xc0, 0x80, 0x00, 0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

            0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            };

        }

        namespace Rotated {

            const uint8_t PROGMEM Player[] = {
            8, 11,
            0x00, 0x20, 0x20, 0x74, 0xdc, 0x8e, 0xf8, 0x54, 
            0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x00, 0x01, 
            };

            const uint8_t PROGMEM Player_Mask[] = {
            0x70, 0x70, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xfe, 
            0x00, 0x00, 0x03, 0x03, 0x07, 0x07, 0x07, 0x03, 
            };

            const uint8_t PROGMEM Player_Explosion[] = {
            12, 19,
            0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x80, 0x00, 0x20, 0x70, 0x20, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x10, 0x39, 0x10, 0x00, 0x40, 0xe0, 0x40, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

            0x00, 0x00, 0x40, 0xe0, 0x40, 0x00, 0x00, 0x08, 0x1c, 0x08, 0x00, 0x00, 
            0x00, 0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x80, 0xc0, 0x80, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 

            0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
            };

        }

    }

    namespace Landscape {

        const uint8_t PROGMEM Player[] = {
        11, 8,
        0x00, 0x20, 0xb8, 0x70, 0xd8, 0x4e, 0xd8, 0x70, 0xb8, 0x20, 0x00, 
        };

        const uint8_t PROGMEM Player_Mask[] = {
        0x70, 0xfc, 0xfc, 0xfc, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xfc, 0x70, 
        };

        const uint8_t PROGMEM Laser[] = {
        5, 7,
        0x00, 0x04, 0x2a, 0x10, 0x00, 
        };

        const uint8_t PROGMEM Laser_Mask[] = {
        0x04, 0x2e, 0x7f, 0x3a, 0x10, 
        };

        const uint8_t PROGMEM Player_Explosion[] = {
        19, 12,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x20, 0x00, 0x00, 0x20, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x02, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x80, 0x00, 0x08, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x08, 0x00, 0x80, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 

        0x00, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        };
        
    }

};
