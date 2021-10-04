#include "MiniRogue.h"

constexpr const static uint8_t Trap_NumberOfCardsInFlip = 13; 

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::trap_Init() {
  
	this->trapScreenVars.init();
	this->counter = 0;
	this->gameState = GameState::Trap;

	this->playSoundEffect(SoundEffect::RollDice);
  
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::trap() {

	switch (this->trapScreenVars.viewState) {

		case Trap_ViewState::SkillCheck:

			if (PC::buttons.pressed(BTN_A)) { 
				for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
					this->trapScreenVars.skillCheck[i] = random(1, 7); 
				}
				counter = sizeof(DiceDelay); 
			}

			if (this->counter < sizeof(DiceDelay)) {

				if (PC::frameCount % DiceDelay[this->counter] == 0) {

					for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
						this->trapScreenVars.skillCheck[i] = random(1, 7); 
					}

					this->counter++;
					PC::frameCount = 0;

				}

			}
			else {    

				this->counter = 0;
				PC::frameCount = 0;
				this->trapScreenVars.viewState = Trap_ViewState::SkillCheckResult;

				for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
					if (this->trapScreenVars.skillCheck[i] >= 5) this->trapScreenVars.hasSkill = true;
				}

				if (this->trapScreenVars.hasSkill) {
					this->playSoundEffect(SoundEffect::Positive);
				}
				else {
					this->playSoundEffect(SoundEffect::Negative);
				}

			}
			break;

		case Trap_ViewState::SkillCheckResult:

			if (PC::buttons.pressed(BTN_A)) { counter = FLASH_COUNTER; }

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}
			else {

				if (PC::buttons.pressed(BTN_A)) {

					if (this->trapScreenVars.hasSkill) {

						uint8_t oldArea = this->gameStats.getAreaId();
						this->gameState = this->gameStats.incRoom(playerStats); 

						if (oldArea != this->gameStats.getAreaId()) {

							this->playTheme(this->gameStats.getAreaId());
							
						}

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
					this->playSoundEffect(SoundEffect::EvilLaugh);

				}

			}
			break;

		case Trap_ViewState::UpdateStats:

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}
			else {

				if (PC::buttons.pressed(BTN_A)) {

					uint8_t oldArea = this->gameStats.getAreaId();
					this->gameState = this->gameStats.incRoom(playerStats); 

					if (oldArea != this->gameStats.getAreaId()) {

						this->playTheme(this->gameStats.getAreaId());
						
					}

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
	// PD::drawBitmap(11, 40, Images::Trap_LHS);
	// PD::drawBitmap(90, 40, Images::Trap_RHS);
	PD::drawBitmap(11, 0, Images::Trap_Background);

	switch (this->trapScreenVars.viewState) {

		case Trap_ViewState::SkillCheck:
		case Trap_ViewState::SkillCheckResult:
			{
				uint8_t left = 56 - ((10 * this->playerStats.getXPTrack()) / 2);

				PD::setColor(7, 1);
				PD::setCursor(this->trapScreenVars.viewState == Trap_ViewState::SkillCheck ? 35 : 24, 3);
				PD::print("Evade trap?");
				PD::setCursor(74, 3);

				for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
					PD::drawBitmap(left + (i * 10), 12, Images::Dice[this->trapScreenVars.skillCheck[i]]);
				}

				if (this->trapScreenVars.viewState == Trap_ViewState::SkillCheckResult) {

					if (this->counter < FLASH_COUNTER && flash) PD::setColor(0);

					if (this->trapScreenVars.hasSkill) {

						if ((this->counter < FLASH_COUNTER) && flash) {
							
							PD::setColor(7, 1);
							PD::fillRect(72, 2, 15, 7);
							PD::setColor(1, 7);

						}
						
						PD::print("Yes");

					}
					else {

						if ((this->counter < FLASH_COUNTER) && flash) {
							
							PD::setColor(7, 1);
							PD::fillRect(72, 2, 11, 7);
							PD::setColor(1, 7);

						}
						
						PD::print("No");

					}

					PD::setColor(7, 1);

				}

			}

			break;

		case Trap_ViewState::RollDice:

			this->renderLargeSpinningCard(39, 8, this->counter);

			if (counter >= Trap_NumberOfCardsInFlip) {

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

			PD::setCursor(15, 1);
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

	PD::setColor(7, 1);

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
