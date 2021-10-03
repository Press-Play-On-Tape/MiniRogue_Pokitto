
#pragma once

namespace Images {

    #include "Card_Mildew.h"
    #include "Card_MovingWalls.h"
    #include "Card_Pit.h"
    #include "Card_RustedArmour.h"
    #include "Card_SpringBlades.h"
    #include "Card_TripWire.h"
    #include "Trap_LHS.h"
    #include "Trap_RHS.h"
    #include "Trap_Single.h"
    #include "Trap_Background.h"

    const uint8_t * const Trap_Dice[] = { 
        Card_Mildew, 
        Card_TripWire, 
        Card_RustedArmour, 
        Card_SpringBlades, 
        Card_MovingWalls, 
        Card_Pit, 
    };

};