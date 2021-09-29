#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;

constexpr const static uint8_t treasure_NumberOfCardsInFlip = 7; 


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::treasure_Init() { 

	this->treasureScreenVars.init();
	this->counter = 0;

	this->gameState = GameState::Treasure;
  
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::treasure() {

	switch (this->treasureScreenVars.viewState) {

		case Treasure_ViewState::InitialRoll:

			if (this->counter < sizeof(DiceDelay)) {

				if (PC::frameCount % DiceDelay[this->counter] == 0) {

					this->treasureScreenVars.dice = random(1, 7);

					if (PC::buttons.pressed(BTN_A)) { 

						counter = sizeof(DiceDelay); 

					}
					else {

						this->counter++;
						PC::frameCount = 0;

					}

				}

			}
			else {

				if (this->treasureScreenVars.dice >= 5) {

					if (PC::buttons.pressed(BTN_A)) {

						this->counter = treasure_NumberOfCardsInFlip;
						this->treasureScreenVars.viewState = Treasure_ViewState::RollDice;
						this->treasureScreenVars.dice = random(1, 7);

					}

				}
				else {

					playerStats.incGold(gameStats.monsterDefeated ? 2 : 1);
					this->counter = 0;
					this->treasureScreenVars.viewState = Treasure_ViewState::UpdateStats;   

				}

			}
			break;

		case Treasure_ViewState::RollDice:

			if (counter > 0) {

				this->treasureScreenVars.dice = random(1, 7);
				this->counter--;

			} 
			else {

				this->treasureScreenVars.foundTreasure = true;
				if (this->playerStats.itemCount() >= 2 && this->treasureScreenVars.dice < 5) this->treasureScreenVars.dice = 7;

				switch (this->treasureScreenVars.dice) {

					case 1 ... 4:   this->playerStats.items[this->treasureScreenVars.dice - 1]++; break;
					case 5:         this->playerStats.incArmour(1); break;
					case 6:         this->playerStats.incXP(2); break;
					case 7:         this->playerStats.incGold(2); break;

				}

				this->counter = 0;
				this->treasureScreenVars.viewState = Treasure_ViewState::UpdateStats;
				this->playerStats.incGold(gameStats.monsterDefeated ? 2 : 1);

			}
			break;


		case Treasure_ViewState::UpdateStats:

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}

			if (PC::buttons.pressed(BTN_A)) {

				this->gameState = gameStats.incRoom(playerStats); 

			}

			break;

	}


	// Render ..

  	const bool flash = PC::frameCount % FLASH_DELAY < (FLASH_DELAY / 2);

	this->renderBackground();
	PD::drawBitmap(11, 0, Images::Treasure_01);

	switch (this->treasureScreenVars.viewState) {

		case Treasure_ViewState::InitialRoll:

			if (this->counter < sizeof(DiceDelay)) {

				PD::drawBitmap(25, 16, Images::Treasure_03);
				PD::drawBitmap(59, 1, Images::Dice[this->treasureScreenVars.dice]);

				PD::setCursor(14, 2);
				PD::print("Open chest:");

			}
			else {

				this->renderChestResults();

			}

			break;

		case Treasure_ViewState::RollDice:

			this->renderSelectTreasure();
			break;

		case Treasure_ViewState::UpdateStats:

			if (this->treasureScreenVars.foundTreasure) {

				this->renderSelectTreasure();

			}
			else {

				this->renderChestResults();

			}

			break;

	}


	// Player statistics ..

	FlashSettings settings = FlashSettings::FlashGold;

	if (this->treasureScreenVars.dice == 6)        		                    					settings = static_cast<FlashSettings>(static_cast<uint8_t>(settings) | static_cast<uint8_t>(FlashSettings::FlashXP));
	else if (this->treasureScreenVars.dice == 5)    		                    				settings = static_cast<FlashSettings>(static_cast<uint8_t>(settings) | static_cast<uint8_t>(FlashSettings::FlashArmour));
	else if (this->treasureScreenVars.dice == 4 && this->treasureScreenVars.foundTreasure)		settings = static_cast<FlashSettings>(static_cast<uint8_t>(settings) | static_cast<uint8_t>(FlashSettings::FlashHP));

	const bool shouldFlash = (this->treasureScreenVars.viewState == Treasure_ViewState::UpdateStats && this->counter < FLASH_COUNTER);

	this->renderPlayerStatistics(shouldFlash, settings);

	if (this->treasureScreenVars.viewState == Treasure_ViewState::UpdateStats && this->treasureScreenVars.foundTreasure && this->counter < FLASH_COUNTER && flash) {

		PD::setCursor(19, 0);
		this->printCaption(this->treasureScreenVars.dice - 1); 

	}

}

void Game::printCaption(uint8_t index) {

	switch (index) {

		case 0:
			PD::print(" Gain a Fire Wand!");
			break;

		case 1:
			PD::print(" Gain an Ice Wand!");
			break;

		case 2:
			PD::print("Gain a Poison Wand!");
			break;

		case 3:
			PD::print("Gain a Healing Wand!");
			break;

		case 4:
			PD::print("  Gain an Armour!");
			break;

		case 5:
			PD::print("Upgrade your Weapon!");
			break;

		case 6:
			PD::print("Gain 2 Gold Pieces!");
			break;

		case 7:
			PD::print("Open chest:");
			PD::setCursor(14, 13);
			PD::print("Gold and treasure!");
			break;

		case 8:
			PD::print("Open chest:");
			PD::setCursor(14, 13);
			PD::print("You found gold only!");
			break;

	}

}

void Game::renderSelectTreasure() {

	PD::drawBitmap(25, 8, Images::Treasure_02);

	if (this->treasureScreenVars.viewState == Treasure_ViewState::RollDice && this->counter > 0) {

		this->renderTreasureSpinningCard(44, 13, this->counter - 1);

	}
	else {

		this->renderTreasureSpinningCard(44, 13, 0);
		PD::drawBitmap(46, 15, Images::Treasure_Dice[this->treasureScreenVars.dice - 1]);

	}

}

void Game::renderChestResults() { 

	PD::setCursor(14, 2);
	PD::drawBitmap(59, 1, Images::Dice[this->treasureScreenVars.dice]);
	PD::drawBitmap(25, 16, Images::Treasure_03);

	if (this->treasureScreenVars.dice >= 5) {

		printCaption(7);

	}
	else {

		printCaption(8);

	}

}
