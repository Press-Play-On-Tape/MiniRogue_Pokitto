
#pragma once

namespace Images {

    #include "BlinkEyes_1.h"
    #include "BlinkEyes_2.h"
    #include "Flames_01.h"
    #include "Flames_02.h"
    #include "Flames_03.h"
    #include "Flames_04.h"
    #include "GameOverBanner.h"
    #include "GameOverSide.h"
    #include "HighScore.h"
    #include "TitleScreen_Blank.h"
    #include "TitleScreen_GameOver.h"
    #include "TitleScreen_MiniRogue.h"
    #include "Winner.h"

    #include "TitleScreen_Levels.h"
    #include "TitleScreen_Levels_00.h"
    #include "TitleScreen_Levels_01.h"
    #include "TitleScreen_Levels_02.h"
    #include "TitleScreen_Levels_03.h"

    const uint8_t * const Flames[] = { 
        Flames_01,
        Flames_02,
        Flames_03,
        Flames_04
    };

    const uint8_t * const Title_Levels[] = { 
        TitleScreen_Levels_00,
        TitleScreen_Levels_01,
        TitleScreen_Levels_02,
        TitleScreen_Levels_03
    };

};