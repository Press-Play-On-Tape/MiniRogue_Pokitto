
#pragma once

namespace Images {

    #include "Card_Food.h"
    #include "Card_Gold.h"
    #include "Card_Monster.h"
    #include "Card_Potion.h"
    #include "Card_Shield.h"
    #include "Card_Whetstone.h"
    #include "Event_Background.h"

    const uint8_t * const Event_Dice[] = { 
        Card_Gold, 
        Card_Potion, 
        Card_Food, 
        Card_Whetstone, 
        Card_Shield, 
        Card_Monster, 
    };

};