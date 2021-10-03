
#pragma once

namespace Images {

    #include "Card_Blank.h"
    #include "Card_Highlight.h"
    #include "CardFace_Boss.h"
    #include "CardFace_Delve.h"
    #include "CardFace_Event.h"
    #include "CardFace_Merchant.h"
    #include "CardFace_Monster.h"
    #include "CardFace_Resting.h"
    #include "CardFace_Trap.h"
    #include "CardFace_Treasure.h"

    #include "spinning/Card_Spinning_01.h"
    #include "spinning/Card_Spinning_02.h"
    #include "spinning/Card_Spinning_03.h"
    #include "spinning/Card_Spinning_04.h"
    #include "spinning/Card_Spinning_05.h"
    #include "spinning/Card_Spinning_06.h"

    #include "treasure/Card_Treasure_Spinning_00.h"
    #include "treasure/Card_Treasure_Spinning_01.h"
    #include "treasure/Card_Treasure_Spinning_02.h"
    #include "treasure/Card_Treasure_Spinning_03.h"
    #include "treasure/Card_Treasure_Spinning_04.h"
    #include "treasure/Card_Treasure_Spinning_05.h"
    #include "treasure/Card_Treasure_Spinning_06.h"

    #include "large/Large_Card_Spinning_01.h"
    #include "large/Large_Card_Spinning_02.h"
    #include "large/Large_Card_Spinning_03.h"
    #include "large/Large_Card_Spinning_04.h"
    #include "large/Large_Card_Spinning_05.h"
    #include "large/Large_Card_Spinning_06.h"
    #include "large/Large_Card_Spinning_07.h"
    #include "large/Large_Card_Spinning_08.h"
    #include "large/Large_Card_Spinning_09.h"
    #include "large/Large_Card_Spinning_10.h"
    #include "large/Large_Card_Spinning_11.h"
    #include "large/Large_Card_Spinning_12.h"
    #include "large/Large_Card_Spinning_13.h"
    #include "large/Large_Card_Spinning_Highlight.h"

    const uint8_t * const Card_Faces[] = { 
        CardFace_Resting,
        CardFace_Treasure,
        CardFace_Event,
        CardFace_Merchant,
        CardFace_Trap,
        CardFace_Monster,
        CardFace_Boss,
        CardFace_Delve
    };

    const uint8_t * const Treasure_Spinning_Card[] = { 
        Card_Treasure_Spinning_06, 
        Card_Treasure_Spinning_05, 
        Card_Treasure_Spinning_04, 
        Card_Treasure_Spinning_03, 
        Card_Treasure_Spinning_02, 
        Card_Treasure_Spinning_01, 
        Card_Treasure_Spinning_00, 
    };

    const uint8_t * const Spinning_Card[] = { 
        Card_Spinning_06, 
        Card_Spinning_05, 
        Card_Spinning_04, 
        Card_Spinning_03, 
        Card_Spinning_02, 
        Card_Spinning_01, 
        Card_Blank, 
    };

    const uint8_t * const Large_Spinning_Cards[] = { 
        // Large_Card_Spinning_13, 
        // Large_Card_Spinning_12, 
        // Large_Card_Spinning_11, 
        // Large_Card_Spinning_10, 
        // Large_Card_Spinning_09, 
        // Large_Card_Spinning_08, 
        // Large_Card_Spinning_07, 
        // Large_Card_Spinning_08, 
        // Large_Card_Spinning_09, 
        // Large_Card_Spinning_10, 
        // Large_Card_Spinning_11, 
        // Large_Card_Spinning_12, 
        // Large_Card_Spinning_13, 
        Large_Card_Spinning_01, 
        Large_Card_Spinning_02, 
        Large_Card_Spinning_03, 
        Large_Card_Spinning_04, 
        Large_Card_Spinning_05, 
        Large_Card_Spinning_06, 
        Large_Card_Spinning_07, 
        Large_Card_Spinning_08, 
        Large_Card_Spinning_09, 
        Large_Card_Spinning_10, 
        Large_Card_Spinning_11, 
        Large_Card_Spinning_12, 
        Large_Card_Spinning_13, 
    };

    const uint8_t Large_Spinning_Inlays[] = { 
        12, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0
    };
};