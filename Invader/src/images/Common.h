#pragma once

namespace Images {

    namespace Portrait {

        const uint8_t PROGMEM Mountain[] = {
        8, 16,
        0x40, 0x20, 0x10, 0x08, 0x04, 0x22, 0x41, 0x80, 
        0x00, 0x02, 0x04, 0x08, 0x10, 0x22, 0x41, 0x80, 
        };

        const uint8_t PROGMEM Mountain_Top[] = {
        8, 16,
        0x01, 0x82, 0x44, 0x08, 0x10, 0x20, 0x40, 0x00, 
        0x01, 0x82, 0x44, 0x20, 0x10, 0x08, 0x04, 0x02, 
        };

        const uint8_t PROGMEM Ground[] = {
        3, 64,
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        0xff, 0x00, 0x55, 
        };

        const uint8_t PROGMEM Ground_Top[] = {
        3, 64,
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        0xaa, 0x00, 0xff, 
        };

        const uint8_t PROGMEM HighScore[] = {
        5, 36,
        0x75, 0x25, 0x27, 0x25, 0x75, 
        0x57, 0x55, 0x75, 0x51, 0x57, 
        0xee, 0x28, 0x2e, 0x22, 0xee, 
        0xae, 0xaa, 0x6a, 0xaa, 0xee, 
        0x0e, 0x02, 0x06, 0x02, 0x0e, 
        };

        const uint8_t PROGMEM Invader[] = {
        9, 54,
        0x00, 0x9e, 0x8c, 0x8c, 0x8c, 0x8c, 0x8e, 0x8c, 0x00, 
        0x00, 0x19, 0x19, 0x9d, 0x9f, 0x9f, 0x9b, 0x99, 0x00, 
        0x00, 0x86, 0x8f, 0x99, 0x99, 0x99, 0x19, 0x19, 0x00, 
        0x00, 0x99, 0x99, 0x99, 0x9f, 0x99, 0x8f, 0x86, 0x00, 
        0x00, 0x87, 0x8d, 0x99, 0x99, 0x99, 0x8d, 0x87, 0x00, 
        0x00, 0x9f, 0x81, 0x81, 0x87, 0x81, 0x81, 0x9f, 0x00, 
        0x00, 0x19, 0x0d, 0x07, 0x0f, 0x19, 0x19, 0x0f, 0x00, 
        };

        const uint8_t PROGMEM DownArrow[] = {
        5, 5,
        0x05, 0x0d, 0x1d, 0x0d, 0x05, 
        0x00, 0x0d, 0x1d, 0x0d, 0x00, 
        0x00, 0x00, 0x1d, 0x00, 0x00,
        };

        const uint8_t PROGMEM Rotate[] = {
        5, 25,
        0x33, 0x51, 0x71, 0x51, 0x33, 
        0x06, 0x84, 0xc4, 0x04, 0x06, 
        0x31, 0x43, 0xf7, 0xe1, 0x46, 
        0x00, 0x00, 0x01, 0x00, 0x00, 
        };
        
        const uint8_t PROGMEM Lock[] = {
        5, 5,
        0x1f, 0x1f, 0x1f, 0x0a, 0x0e, 
        };

        const uint8_t PROGMEM SurvivalMode[] = {
        5, 41,
        0x17, 0x12, 0x72, 0x53, 0x72, 
        0xdc, 0x50, 0x5c, 0x44, 0x5c, 
        0x95, 0x55, 0x4d, 0x55, 0x5d, 
        0x9c, 0x49, 0x49, 0x49, 0x5d, 
        0xd4, 0x55, 0x5d, 0x55, 0x5d, 
        0x01, 0x00, 0x00, 0x00, 0x00, 
        };

        const uint8_t PROGMEM VSMode[] = {
        5, 37,
        0x17, 0x11, 0x77, 0x54, 0x77, 
        0xc8, 0x14, 0xd4, 0x54, 0xd4, 
        0xa1, 0xa1, 0xe1, 0xe0, 0xa1, 
        0x6e, 0xaa, 0xaa, 0xaa, 0x6e, 
        0x0e, 0x02, 0x06, 0x02, 0x0e, 
        };

        const uint8_t PROGMEM TugOfWarMode[] = {
        5, 45,
        0x17, 0x11, 0x77, 0x54, 0x77, 
        0xc8, 0x48, 0x48, 0x48, 0x5c, 
        0x19, 0x15, 0x15, 0x05, 0x19, 
        0x17, 0x15, 0x35, 0x15, 0x77, 
        0x54, 0xdc, 0x54, 0x54, 0xd4, 
        0x15, 0x15, 0x0d, 0x15, 0x1d, 
        };

        const uint8_t PROGMEM WaveCleared_00[] = {
        8, 8,
        0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_01[] = {
        8, 12,
        0xfa, 0x1a, 0xda, 0xda, 0xda, 0xda, 0xfa, 0x00, 
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_02[] = {
        8, 20,
        0xfa, 0x1a, 0xda, 0xda, 0xda, 0x1a, 0xfa, 0x00, 
        0xff, 0x11, 0xdd, 0x9d, 0xdd, 0x1d, 0xff, 0x00, 
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_03[] = {
        8, 28,
        0xfa, 0x8a, 0xea, 0xca, 0xea, 0x8a, 0xfa, 0x00, 
        0xff, 0x11, 0xdd, 0xdd, 0xdd, 0xd1, 0xff, 0x00, 
        0xff, 0x51, 0x5d, 0x19, 0x5d, 0x11, 0xff, 0x00, 
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_04[] = {
        8, 36,
        0xfa, 0xda, 0xaa, 0xaa, 0xaa, 0xaa, 0xfa, 0x00, 
        0xff, 0x18, 0xde, 0xdc, 0xde, 0x18, 0xff, 0x00, 
        0xff, 0x11, 0xdd, 0x9d, 0xdd, 0x1d, 0xff, 0x00, 
        0xff, 0x55, 0x55, 0x91, 0x55, 0x11, 0xff, 0x00, 
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_05[] = {
        8, 44,
        0xfa, 0xaa, 0xaa, 0x8a, 0xaa, 0xda, 0xfa, 0x00, 
        0xff, 0x8d, 0xea, 0xca, 0xea, 0x8a, 0xff, 0x00, 
        0xff, 0x11, 0xdd, 0xdd, 0xdd, 0xd1, 0xff, 0x00, 
        0xff, 0x51, 0x5d, 0x19, 0x5d, 0x11, 0xff, 0x00, 
        0xff, 0x15, 0xd5, 0x99, 0xd5, 0x11, 0xff, 0x00, 
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_06[] = {
        8, 52,
        0xfa, 0xaa, 0x8a, 0xaa, 0xaa, 0xaa, 0xfa, 0x00, 
        0xff, 0xda, 0xaa, 0xa8, 0xaa, 0xad, 0xff, 0x00, 
        0xff, 0x18, 0xde, 0xdc, 0xde, 0x18, 0xff, 0x00, 
        0xff, 0x11, 0xdd, 0x9d, 0xdd, 0x1d, 0xff, 0x00, 
        0xff, 0x55, 0x55, 0x91, 0x55, 0x11, 0xff, 0x00, 
        0xff, 0x91, 0x5d, 0x59, 0x5d, 0x91, 0xff, 0x00, 
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 
        };

        const uint8_t * const WaveCleared[] = { 
            WaveCleared_00,
            WaveCleared_01,
            WaveCleared_02,
            WaveCleared_03,
            WaveCleared_04,
            WaveCleared_05,
            WaveCleared_06,
        };

    }

    namespace Landscape {

        const uint8_t PROGMEM Mountain[] = {
        16, 8,
        0x02, 0x04, 0x08, 0x10, 0x20, 0x44, 0x82, 0x01, 0x02, 0x44, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 
        };

        const uint8_t PROGMEM Ground[] = {
        8, 3,
        0x05, 0x04, 0x05, 0x04, 0x05, 0x04, 0x05, 0x04, 
        };

        const uint8_t PROGMEM HighScore[] = {
        36, 5,
        0x1f, 0x04, 0x1f, 0x00, 0x11, 0x1f, 0x11, 0x00, 0x1f, 0x11, 0x1d, 0x00, 0x1f, 0x04, 0x1f, 0x00, 0x00, 0x17, 0x15, 0x1d, 0x00, 0x1f, 0x11, 0x11, 0x00, 0x1f, 0x11, 0x1f, 0x00, 0x1f, 0x05, 0x1b, 0x00, 0x1f, 0x15, 0x11, 
        };

        const uint8_t PROGMEM Invader[] = {
        54, 9,
        0x00, 0x84, 0xfe, 0xfe, 0x80, 0x00, 0x00, 0xfe, 0xfe, 0x1c, 0x38, 0xfe, 0xfe, 0x00, 0x00, 0x3e, 0x7e, 0xc0, 0xc0, 0x7e, 0x3e, 0x00, 0x00, 0xf8, 0xfc, 0x16, 0x16, 0xfc, 0xf8, 0x00, 0x00, 0xfe, 0xfe, 0x82, 0xc6, 0x7c, 0x38, 0x00, 0x00, 0xfe, 0xfe, 0x92, 0x92, 0x82, 0x82, 0x00, 0x00, 0xfe, 0xfe, 0x32, 0x72, 0xde, 0x8c, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        };

        const uint8_t PROGMEM LeftArrow[] = {
        5, 5,
        0x1f, 0x00, 0x1f, 0x0e, 0x04, 
        0x0e, 0x00, 0x0e, 0x0e, 0x04, 
        0x04, 0x00, 0x04, 0x04, 0x04, 
        };
  
        const uint8_t PROGMEM Rotate[] = {
        25, 5,
        0x1f, 0x11, 0x00, 0x00, 0x1f, 0x15, 0x0e, 0x00, 0x00, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x04, 0x0c, 0x1e, 0x0d, 0x05, 0x00, 0x14, 0x16, 0x0f, 0x06, 0x04, 
        };

        const uint8_t PROGMEM Lock[] = {
        5, 5,
        0x1c, 0x1f, 0x1d, 0x1f, 0x1c, 
        };

        const uint8_t PROGMEM SurvivalMode[] = {
        41, 5,
        0x12, 0x1f, 0x10, 0x00, 0x1f, 0x05, 0x07, 0x00, 0x00, 0x00, 0x17, 0x15, 0x1d, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x1f, 0x05, 0x1b, 0x00, 0x0f, 0x10, 0x0f, 0x00, 0x11, 0x1f, 0x11, 0x00, 0x0f, 0x10, 0x0f, 0x00, 0x1f, 0x05, 0x1f, 0x00, 0x1f, 0x10, 0x10, 
        };

        const uint8_t PROGMEM VSMode[] = {
        37, 5,
        0x1d, 0x15, 0x17, 0x00, 0x1f, 0x05, 0x07, 0x00, 0x00, 0x00, 0x0f, 0x10, 0x0f, 0x00, 0x17, 0x15, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x06, 0x1f, 0x00, 0x1f, 0x11, 0x1f, 0x00, 0x1f, 0x11, 0x0e, 0x00, 0x1f, 0x15, 0x11, 0x00, 
        };

        const uint8_t PROGMEM TugOfWarMode[] = {
        45, 5,
        0x1d, 0x15, 0x17, 0x00, 0x1f, 0x05, 0x07, 0x00, 0x00, 0x00, 0x01, 0x1f, 0x01, 0x00, 0x1f, 0x10, 0x1f, 0x00, 0x0e, 0x11, 0x1d, 0x00, 0x00, 0x00, 0x1f, 0x11, 0x1f, 0x00, 0x1f, 0x05, 0x01, 0x00, 0x00, 0x00, 0x1f, 0x08, 0x1f, 0x00, 0x1f, 0x09, 0x1f, 0x00, 0x1f, 0x05, 0x1b, 
        };


        const uint8_t PROGMEM WaveCleared_00[] = {
        8, 8,
        0x00, 0xfe, 0x00, 0xfe, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_01[] = {
        12, 8,
        0x00, 0xfe, 0x00, 0xfe, 0xfe, 0x82, 0xbe, 0xbe, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_02[] = {
        20, 8,
        0x00, 0xfe, 0x00, 0xfe, 0xfe, 0x82, 0xba, 0xba, 0xfe, 0x82, 0xbe, 0xbe, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_03[] = {
        28, 8,
        0x00, 0xfe, 0x00, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0xfe, 0x82, 0xba, 0xba, 0xfe, 0x82, 0xbe, 0xbe, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x82, 0xea, 0x82, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_04[] = {
        36, 8,
        0x00, 0xfe, 0x00, 0xfe, 0xc2, 0xbe, 0xc2, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0xfe, 0x82, 0xba, 0xba, 0xfe, 0x82, 0xbe, 0xbe, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x82, 0xea, 0x82, 0xfe, 0x82, 0xea, 0x92, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_05[] = {
        44, 8,
        0x00, 0xfe, 0x00, 0xfe, 0x86, 0xea, 0x86, 0xfe, 0xc2, 0xbe, 0xc2, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0xfe, 0x82, 0xba, 0xba, 0xfe, 0x82, 0xbe, 0xbe, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x82, 0xea, 0x82, 0xfe, 0x82, 0xea, 0x92, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t PROGMEM WaveCleared_06[] = {
        52, 8,
        0x00, 0xfe, 0x00, 0xfe, 0x82, 0xde, 0x82, 0xfe, 0x86, 0xea, 0x86, 0xfe, 0xc2, 0xbe, 0xc2, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0xfe, 0x82, 0xba, 0xba, 0xfe, 0x82, 0xbe, 0xbe, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x82, 0xea, 0x82, 0xfe, 0x82, 0xea, 0x92, 0xfe, 0x82, 0xaa, 0xba, 0xfe, 0x82, 0xba, 0xc6, 0xfe, 0x00, 0xfe, 0x00, 
        };

        const uint8_t * const WaveCleared[] = { 
            WaveCleared_00,
            WaveCleared_01,
            WaveCleared_02,
            WaveCleared_03,
            WaveCleared_04,
            WaveCleared_05,
            WaveCleared_06,
        };

    }

}