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

	if (this->currentTheme != 0) {

		this->playTheme(0);
		
	}

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::title() { 


	// Handle input ..

	if (PC::buttons.pressed(BTN_LEFT) && this->gameStats.skillLevel > MIN_LEVEL)        --this->gameStats.skillLevel;
	if (PC::buttons.pressed(BTN_RIGHT) && this->gameStats.skillLevel < MAX_LEVEL + 1)   ++this->gameStats.skillLevel;

	if (PC::buttons.pressed(BTN_A) && this->gameStats.skillLevel <= MAX_LEVEL) {

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

		// this->gameStats.level = 13;//SJH
		
	}


	if (this->gameStats.skillLevel == MAX_LEVEL + 1) {
			
		if (PC::buttons.pressed(BTN_UP)) {

			this->cookie->sfx--;
			this->cookie->saveCookie();

			if (this->cookie->sfx != SoundEffects::Both && this->cookie->sfx != SoundEffects::Music) {

				this->muteTheme();
				
			}
			else {

				this->playTheme(0);

			}

		}

		if (PC::buttons.pressed(BTN_DOWN)) {

			this->cookie->sfx++;
			this->cookie->saveCookie();

			if (this->cookie->sfx != SoundEffects::Both && this->cookie->sfx != SoundEffects::Music) {

				this->muteTheme();
				
			}
			else {

				this->playTheme(0);
				
			}
			
		}

	}


    PD::drawBitmap(-9, 0, Images::TitleScreen_Blank);
    PD::drawBitmap(55, 0, Images::TitleScreen_Blank, NOROT, FLIPH);

    PD::drawBitmap(-4, 25, Images::Flames[this->leftFlame]);
    PD::drawBitmap(98, 25, Images::Flames[this->rightFlame], NOROT, FLIPH);

    if (PC::frameCount % 3 == 0) { 
        this->leftFlame = (this->leftFlame + 1) % 4;
        this->rightFlame = (this->rightFlame + 1) % 4;
    }

	PD::setColor(7);
	PD::drawFastHLine(10, 68, 90);
	PD::drawFastHLine(10, 70, 90);
	this->drawHorizontalDottedLine(10, 98, 72, 6);

	PD::drawBitmap(20, 24, Images::TitleScreen_MiniRogue);
	PD::drawBitmap(9, 76, Images::TitleScreen_Levels);

	if (PC::frameCount % 70 < 7) {
		PD::drawBitmap(42, 8, Images::BlinkEyes_2);
		PD::drawBitmap(60, 8, Images::BlinkEyes_2, NOROT, FLIPH);
	}

	static const uint8_t xPos[] = { 9, 30, 60, 80 };

	if (this->gameStats.skillLevel <= MAX_LEVEL) {

		PD::drawBitmap(xPos[gameStats.skillLevel], 76, Images::Title_Levels[this->gameStats.skillLevel]);

		switch (this->cookie->sfx) {

			case SoundEffects::Music:
				PD::drawBitmap(80, 76, Images::Sound_Music_Grey);
				break;

			case SoundEffects::SFX:
				PD::drawBitmap(80, 76, Images::Sound_SFX_Grey);
				break;

			case SoundEffects::Both:
				PD::drawBitmap(80, 76, Images::Sound_Both_Grey);
				break;

			default:
				PD::drawBitmap(80, 76, Images::Sound_None_Grey);
				break;

		}
	
	}
	else {

		switch (this->cookie->sfx) {

			case SoundEffects::Music:
				PD::drawBitmap(80, 76, Images::Sound_Music_White);
				break;

			case SoundEffects::SFX:
				PD::drawBitmap(80, 76, Images::Sound_SFX_White);
				break;

			case SoundEffects::Both:
				PD::drawBitmap(80, 76, Images::Sound_Both_White);
				break;

			default:
				PD::drawBitmap(80, 76, Images::Sound_None_White);
				break;

		}

	}

	PD::drawBitmap(xPos[gameStats.skillLevel] - 2, 82, Images::Title_Cursors[this->gameStats.skillLevel]);

}
