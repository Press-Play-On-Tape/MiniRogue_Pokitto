#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;



// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::splash_Init() {

    this->gameState = GameState::SplashScreen;
    this->splashScreenVariables.init();
    PC::buttons.pollButtons();

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::splash() { 

    auto justPressed = PC::buttons.pressed(BTN_A) || PC::buttons.pressed(BTN_B) || PC::buttons.pressed(BTN_C);

    if (justPressed > 0) {

        this->gameState = GameState::TitleScreen_Init; 

    }
    
    if (PC::frameCount % 12 == 0) {

        this->splashScreenVariables.counter++;
        
        if (this->splashScreenVariables.counter == 4) {
            
            this->splashScreenVariables.counter = 0;
            
        }
        
    }
    

    //  Render the state ..

    PD::drawBitmap(22, 27, Images::Ppot_Full);
    PD::drawBitmap(32, 38, Images::Ppot[this->splashScreenVariables.counter]);

    if (this->splashScreenVariables.counter < 2) PD::drawBitmap(81, 36, Images::Ppot_Arrow);

}
