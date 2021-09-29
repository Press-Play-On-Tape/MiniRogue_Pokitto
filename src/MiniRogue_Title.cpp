#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;



// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::title_Init() {

    this->gameState = GameState::TitleScreen;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::title() { 


	// Handle input ..

	if (PC::buttons.pressed(BTN_LEFT) && this->gameStats.skillLevel > MIN_LEVEL)        --this->gameStats.skillLevel;
	if (PC::buttons.pressed(BTN_RIGHT) && this->gameStats.skillLevel < MAX_LEVEL) 	    ++this->gameStats.skillLevel;

	if (PC::buttons.pressed(BTN_A)) {

		constexpr const static uint8_t ELEMENTS_PER_ROW = 4;

		uint8_t idx = this->gameStats.skillLevel * ELEMENTS_PER_ROW;
		this->playerStats.xp = 0;
		playerStats.armour = InitSettings[idx++];
		playerStats.hp = InitSettings[idx++];
		playerStats.gold = InitSettings[idx++];
		playerStats.food = InitSettings[idx];
		this->playerStats.xpTrack = 1;

		this->resetGame();
        this->gameState = GameState::ShowCards_Init;
		
	}


	this->renderTitleBackground(true);
	PD::drawBitmap(20, 16, Images::TitleScreen_MiniRogue);
	PD::drawBitmap(10, 56, Images::TitleScreen_Levels);

	if (PC::frameCount % 70 < 7) {
		PD::drawBitmap(42, 4, Images::BlinkEyes_2);
	}
	else {
		PD::drawBitmap(42, 4, Images::BlinkEyes_1);
	}

	{

        static const uint8_t xPos[] = { 10, 33, 65, 87 };
        static const uint8_t width[] = { 15, 23, 14, 11 };

		PD::drawFastHLine(xPos[gameStats.skillLevel], 63, width[gameStats.skillLevel]);

	}


}
