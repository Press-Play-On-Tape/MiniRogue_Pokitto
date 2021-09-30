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


    PD::drawBitmap(-9, 0, Images::TitleScreen_Blank);
    PD::drawBitmap(55, 0, Images::TitleScreen_Blank, NOROT, FLIPH);

    PD::drawBitmap(-4, 25, Images::Flames[this->leftFlame]);
    PD::drawBitmap(98, 25, Images::Flames[this->rightFlame], NOROT, FLIPH);

    if (PC::frameCount % 3 == 0) { 
        this->leftFlame = (this->leftFlame + 1) % 4;
        this->rightFlame = (this->rightFlame + 1) % 4;
    }

	PD::drawFastHLine(8, 65, 94);
	PD::drawFastHLine(8, 67, 94);
	this->drawHorizontalDottedLine(8, 100, 62, 1);

	PD::drawBitmap(20, 25, Images::TitleScreen_MiniRogue);
	PD::drawBitmap(10, 76, Images::TitleScreen_Levels);

	if (PC::frameCount % 70 < 7) {
		PD::drawBitmap(42, 8, Images::BlinkEyes_2);
		PD::drawBitmap(60, 8, Images::BlinkEyes_2, NOROT, FLIPH);
	}

	static const uint8_t xPos[] = { 10, 33, 65, 87 };
	static const uint8_t width[] = { 15, 23, 14, 11 };

	PD::drawBitmap(xPos[gameStats.skillLevel], 76, Images::Title_Levels[this->gameStats.skillLevel]);
	PD::drawFastHLine(xPos[gameStats.skillLevel], 82, width[this->gameStats.skillLevel]);

}
