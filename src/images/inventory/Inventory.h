#pragma once

namespace Images {

    #include "Border.h"
    #include "FireWand.h"
    #include "HealingWand.h"
    #include "IceWand.h"
    #include "PoisonWand.h"

    const uint8_t * const Wands[] = { 
        FireWand, 
        IceWand, 
        PoisonWand, 
        HealingWand, 
    };

};