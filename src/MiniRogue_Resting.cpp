#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::resting_Init() {

	this->restingScreenVars.init();
	this->counter = 0;
	this->gameState = GameState::Resting;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::resting() { 

	switch (this->restingScreenVars.viewState) {	

		case Resting_ViewState::SelectReward:

			if (PC::buttons.pressed(BTN_LEFT) && this->restingScreenVars.selectedItem == Resting_SelectedItem::Weapon)     { this->restingScreenVars.selectedItem = this->restingScreenVars.prevSelection; }
			if (PC::buttons.pressed(BTN_RIGHT) && this->restingScreenVars.selectedItem != Resting_SelectedItem::Weapon)    { this->restingScreenVars.prevSelection = this->restingScreenVars.selectedItem; this->restingScreenVars.selectedItem = Resting_SelectedItem::Weapon; }
			if (PC::buttons.pressed(BTN_DOWN) && this->restingScreenVars.selectedItem == Resting_SelectedItem::Food)       { this->restingScreenVars.selectedItem = Resting_SelectedItem::Heal; }
			if (PC::buttons.pressed(BTN_UP) && this->restingScreenVars.selectedItem == Resting_SelectedItem::Heal)         { this->restingScreenVars.selectedItem = Resting_SelectedItem::Food; }

			if (PC::buttons.pressed(BTN_A))   { 

				switch (this->restingScreenVars.selectedItem) {

					case Resting_SelectedItem::Weapon:
						this->playerStats.incXP(1);
						break;

					case Resting_SelectedItem::Food:
						this->playerStats.food++;
						break;

					case Resting_SelectedItem::Heal:
						this->playerStats.incHP(2);
						break;

				}

				this->restingScreenVars.viewState = Resting_ViewState::UpdateStats;

			}

			break;

		case Resting_ViewState::UpdateStats:

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

				if (counter == FLASH_COUNTER) {

					this->gameState = this->gameStats.incRoom(playerStats); 

				}

			}

			if (PC::buttons.pressed(BTN_A)) {

				this->gameState = this->gameStats.incRoom(playerStats); 

			}

			break;

  	}


	// Render ..

	const bool flash = PC::frameCount % FLASH_DELAY < (FLASH_DELAY / 2);

	this->renderBackground();
	PD::drawBitmap(11, 0, Images::Card_Resting_Large);


	// Player statistics ..

	static const FlashSettings settingsHelper[] = 
	{
		FlashSettings::FlashFood,
		FlashSettings::FlashHP,
		FlashSettings::FlashXP,
	};

	const uint8_t index = static_cast<uint8_t>(this->restingScreenVars.selectedItem);
	const FlashSettings settings = static_cast<FlashSettings>(settingsHelper[index]);
	const bool shouldFlash = (this->restingScreenVars.viewState == Resting_ViewState::UpdateStats);

	this->renderPlayerStatistics(shouldFlash, settings);

	if (flash) {

		uint8_t a = 11;
		uint8_t b = 59;
		uint8_t c = 0;
		uint8_t d = 34;

		switch (this->restingScreenVars.selectedItem) {

			case Resting_SelectedItem::Food:
				break;

			case Resting_SelectedItem::Heal:
				c = 36;
				d = 63;
				break;

			case Resting_SelectedItem::Weapon:
				a = 61;
				b = 98;
				d = 63;
				break;

		}

		PD::setColor(0);
		this->drawHorizontalDottedLine(a, b, c, 0);
		this->drawHorizontalDottedLine(a, b, d, 0);
		this->drawVerticalDottedLine(c, d, a, 0);
		this->drawVerticalDottedLine(c, d, b, 0);

	}

}
