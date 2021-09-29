#include "MiniRogue.h"

constexpr const static uint8_t Trap_NumberOfCardsInFlip = 13; 

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::trap_Init() {
  
	this->trapScreenVars.init();
	this->counter = 0;
	this->gameState = GameState::Trap;
  
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::trap() {

	switch (this->trapScreenVars.viewState) {

		case Trap_ViewState::SkillCheck:

			if (PC::buttons.pressed(BTN_A)) { this->trapScreenVars.dice = random(1, 7); counter = sizeof(DiceDelay); }

				if (this->counter < sizeof(DiceDelay)) {

				if (PC::frameCount % DiceDelay[this->counter] == 0) {

					this->trapScreenVars.dice = random(1, 7);
					this->counter++;
					PC::frameCount = 0;

				}

			}
			else {    

				this->counter = 0;
				PC::frameCount = 0;
				this->trapScreenVars.viewState = Trap_ViewState::SkillCheckResult;

			}
			break;

		case Trap_ViewState::SkillCheckResult:

			if (PC::buttons.pressed(BTN_A)) { counter = FLASH_COUNTER; }

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}
			else {

				if (PC::buttons.pressed(BTN_A)) {

					if (this->trapScreenVars.dice <= playerStats.xpTrack) {

						this->gameState = gameStats.incRoom(playerStats); 

					}
					else {

						this->counter = 0;
						this->trapScreenVars.viewState = Trap_ViewState::RollDice;

					}

				}

			}
			break;

		case Trap_ViewState::RollDice:

			if (counter < Trap_NumberOfCardsInFlip - 1) {

				this->trapScreenVars.dice = random(1, 7);
				counter++;

			}
			else {

				switch (this->trapScreenVars.dice) {

					case 1: playerStats.decFood(1);       break;
					case 2: playerStats.decGold(1);       break;
					case 3: playerStats.decArmour(1);     break;
					case 4: playerStats.decHP(1);         break;
					case 5: if (playerStats.xp > 1)       playerStats.xp--;       break;

					case 6: 

						playerStats.decHP(2); 

						if (playerStats.hp > 0) {
							gameStats.dropArea();
						}

					break;

				}


				// Have we died?

				if (playerStats.hp > 0) {

					PC::frameCount = 0;
					this->counter = 0;
					this->trapScreenVars.viewState = Trap_ViewState::UpdateStats;

				}
				else {

					trapScreenVars.viewState = Trap_ViewState::PlayerDead;

				}

			}
			break;

		case Trap_ViewState::UpdateStats:

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}
			else {

				if (PC::buttons.pressed(BTN_A)) {

					this->gameState = gameStats.incRoom(playerStats); 

				}

			}

			break;

		case Trap_ViewState::PlayerDead:

			if (PC::buttons.pressed(BTN_A)) {
				this->gameState = GameState::GameOver_Init;
			}

			break;

	}


	// Render screen ..

	const bool flash = PC::frameCount % FLASH_DELAY < (FLASH_DELAY / 2);


	// Render common parts ..

	this->renderBackground();
	PD::drawBitmap(11, 40, Images::Trap_LHS);
	PD::drawBitmap(90, 40, Images::Trap_RHS);

	for (uint8_t i = 20; i < 81; i = i + 14) {
		PD::drawBitmap(i, 40, Images::Trap_Single);
	}

	switch (this->trapScreenVars.viewState) {

		case Trap_ViewState::SkillCheck:
		case Trap_ViewState::SkillCheckResult:

			PD::setCursor(14, 3);
			PD::print("Evade trap?");
			PD::setCursor(77, 3);
			PD::drawBitmap(62, 2, Images::Dice[this->trapScreenVars.dice]);

			if (this->trapScreenVars.viewState == Trap_ViewState::SkillCheckResult) {

				if (this->counter < FLASH_COUNTER && flash) PD::setColor(0);

				if (this->trapScreenVars.dice <= playerStats.xpTrack) {

					if ((this->counter < FLASH_COUNTER) && flash) {
						
						PD::setColor(7);
						PD::fillRect(75, 2, 15, 7);
						PD::setColor(0, 7);

					}
					
					PD::print("Yes");

				}
				else {

					if ((this->counter < FLASH_COUNTER) && flash) {
						
						PD::setColor(7);
						PD::fillRect(75, 2, 11, 7);
						PD::setColor(0, 7);

					}
					
					PD::print("No");

				}

				PD::setColor(7, 0);

			}

			break;

		case Trap_ViewState::RollDice:

			this->renderLargeSpinningCard(39, 8, this->counter);

			if (counter < Trap_NumberOfCardsInFlip) {

				for (uint8_t i = 0, j = 0; i < Images::Large_Spinning_Inlays[this->counter]; i++, j = j + 2) {
					PD::drawBitmap(43 + (this->counter * 2) + j, 8, Images::Large_Card_Spinning_Inlay);
				}

			}
			else {

				PD::drawBitmap(41, 10, Images::Dice[this->trapScreenVars.dice - 1]);

			}

			break;

		case Trap_ViewState::UpdateStats:

			this->renderLargeSpinningCard(39, 8, 0);
			PD::drawBitmap(41, 10, Images::Trap_Dice[this->trapScreenVars.dice - 1]);
			PD::setCursor(15, 0);
			printTrapName();

			break;

		case Trap_ViewState::PlayerDead:

			PD::setCursor(15, 0);
			printTrapName();
			this->renderLargeSpinningCard(39, 8, 0);
			PD::drawBitmap(41, 10, Images::Trap_Dice[this->trapScreenVars.dice - 1]);
			this->renderPlayerDead(35);
			break;

	}	

	static const FlashSettings diceHelper[] = {
		FlashSettings::None,
		FlashSettings::FlashFood,
		FlashSettings::FlashGold,
		FlashSettings::FlashArmour,
		FlashSettings::FlashHP,
		FlashSettings::FlashXP,
		FlashSettings::FlashHP,
	};

	// Player statistics ..

	const FlashSettings settings = static_cast<FlashSettings>(diceHelper[this->trapScreenVars.dice]);
	const bool shouldFlash = (this->trapScreenVars.viewState == Trap_ViewState::UpdateStats && this->counter < FLASH_COUNTER);

	this->renderPlayerStatistics(shouldFlash, settings);

}

void Game::printTrapName() {

	switch (this->trapScreenVars.dice - 1) {

		case 0:
			PD::print("Mildew        -1 Food");
			break;

		case 1:
			PD::print("Tripwire      -1 Gold");
			break;

		case 2:
			PD::print("Rusted Armour   -1 AR");
			break;

		case 3:
			PD::print("Spring Blade    -1 HP");
			break;

		case 4:
			PD::print("Moving Walls    -1 XP");
			break;

		case 5:
			PD::print("Pit      -2 HP, +Fall");
			break;
			
	}

}
