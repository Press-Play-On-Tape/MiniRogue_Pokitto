#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;

static const uint8_t Event_NumberOfCardsInFlip = 13; 
static const uint8_t Event_CardDrawOrder[] = { 2, 1, 0, 2, 0, 1, 0, 1, 2 };
static const uint8_t Event_ImageX[] = { 13, 39, 65 };
static const uint8_t Event_ImageY[] = { 6, 14, 6 };


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::event_Init() {

	this->eventScreenVars.init();
	this->counter = 0;
	this->gameState = GameState::Event;

	this->playSoundEffect(SoundEffect::RollDice);

}

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::event() { 

	switch (this->eventScreenVars.viewState) {

		case Event_ViewState::SkillCheck:

			if (PC::buttons.pressed(BTN_A)) { 
				for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
					this->eventScreenVars.skillCheck[i] = random(1, 7); 
				}
				counter = FLASH_COUNTER; 
			}

			if (this->counter < sizeof(DiceDelay)) {

				if (PC::frameCount % DiceDelay[this->counter] == 0) {

					for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
						this->eventScreenVars.skillCheck[i] = random(1, 7);
					}
					this->counter++;
					PC::frameCount = 0;

				}

			}
			else {

				this->counter = 0;
				PC::frameCount = 0;
				this->eventScreenVars.viewState = Event_ViewState::SkillCheckResult;
					
				for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
					if (this->eventScreenVars.skillCheck[i] >= 5) this->eventScreenVars.hasSkill = true;
				}

				if (this->eventScreenVars.hasSkill) {
					this->playSoundEffect(SoundEffect::Positive);
				}
				else {
					this->playSoundEffect(SoundEffect::Negative);
				}
					
			}
			break;

		case Event_ViewState::SkillCheckResult:

			if (PC::buttons.pressed(BTN_A)) { counter = FLASH_COUNTER; }

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}
			else {

				if (PC::buttons.pressed(BTN_A)) {

					this->counter = 0;
					this->eventScreenVars.viewState = Event_ViewState::RollDice;

					if (this->eventScreenVars.hasSkill) {

						this->eventScreenVars.nextState = Event_ViewState::SelectCard;

					}
					else {

						this->eventScreenVars.nextState = Event_ViewState::UpdateStats;

					}

				}

			}
			break;

		case Event_ViewState::RollDice:

			if (this->counter < Event_NumberOfCardsInFlip - 1) {

				this->eventScreenVars.dice[1] = random(1, 7);
				this->eventScreenVars.dice[0] = this->eventScreenVars.dice[1] - 1;  if (this->eventScreenVars.dice[0] < 1) this->eventScreenVars.dice[0] = 6;
				this->eventScreenVars.dice[2] = this->eventScreenVars.dice[1] + 1;  if (this->eventScreenVars.dice[2] > 6) this->eventScreenVars.dice[2] = 1;
				this->counter++;

			}      
			else {

				this->eventScreenVars.selection = 1;
				this->eventScreenVars.viewState = this->eventScreenVars.nextState;
				this->counter = 0;

			}
			break;

		case Event_ViewState::SelectCard:

			if (PC::buttons.pressed(BTN_LEFT) && this->eventScreenVars.selection > 0)   { this->eventScreenVars.selection--; } 
			if (PC::buttons.pressed(BTN_RIGHT) && this->eventScreenVars.selection < 2)  { this->eventScreenVars.selection++; } 

			if (PC::buttons.pressed(BTN_A))  { 

				this->counter = 0;
				this->eventScreenVars.viewState = Event_ViewState::UpdateStats;

			} 

			break;

		case Event_ViewState::UpdateStats:
	
			if (counter == 0) {

				switch (this->eventScreenVars.dice[this->eventScreenVars.selection]) {

					case 1: playerStats.incGold(2);     break;
					case 2: playerStats.incHP(2);       break;
					case 3: playerStats.incFood(1);     break;
					case 4: playerStats.incXP(2);       break;
					case 5: playerStats.incArmour(1);   break;
					case 6:                             break;

				}

			}

			if (PC::buttons.pressed(BTN_A)) { this->counter = FLASH_COUNTER;}

			if (this->counter < FLASH_COUNTER) {

				this->counter++;

			}
			else {

				if (PC::buttons.pressed(BTN_A)) { 

					if (this->eventScreenVars.dice[this->eventScreenVars.selection] < 6) {

						uint8_t oldArea = this->gameStats.getAreaId();
						this->gameState = this->gameStats.incRoom(playerStats); 

						if (oldArea != this->gameStats.getAreaId()) {

							this->playTheme(this->gameStats.getAreaId());
							
						}

					}
					else {
						
						this->gameState = GameState::MonsterFromEvent_Init;
					}

				}

			}

			break;

	}


	// Render

	const bool flash = PC::frameCount % FLASH_DELAY < (FLASH_DELAY / 2);


	// Render common parts ..

	this->renderBackground();

	PD::drawBitmap(11, 0, Images::Event_Background);


	switch (this->eventScreenVars.viewState) {

		case Event_ViewState::RollDice:

			if (this->eventScreenVars.nextState == Event_ViewState::SelectCard) {
				this->renderLargeSpinningCard(13, 6, this->counter);
				this->renderLargeSpinningCard(65, 6, this->counter);
			}

			this->renderLargeSpinningCard(39, 14, this->counter);

			if (counter >= Event_NumberOfCardsInFlip) {

				PD::drawBitmap(41, 10, Images::Event_Dice[this->eventScreenVars.dice[1] - 1]);

			}

			break;

		case Event_ViewState::SkillCheck:
		case Event_ViewState::SkillCheckResult:
			{

				uint8_t left = 56 - ((10 * this->playerStats.getXPTrack()) / 2);

				PD::setColor(7, 1);
				PD::setCursor(this->eventScreenVars.viewState == Event_ViewState::SkillCheck ? 30 : 21, 3);
				PD::print("Enough skill?");
				PD::setCursor(77, 3);

				for (uint8_t i = 0; i < this->playerStats.getXPTrack(); i++) {
					PD::drawBitmap(left + (i * 10), 12, Images::Dice[this->eventScreenVars.skillCheck[i]]);
				}

				if (this->eventScreenVars.viewState == Event_ViewState::SkillCheckResult) {

					if (this->counter < FLASH_COUNTER && flash) {

						PD::setColor(7);
						PD::fillRect(75, 2, (this->eventScreenVars.hasSkill ? 15 : 11), 7);
						PD::setColor(0, 7);

					}

					if (this->eventScreenVars.hasSkill) {

						PD::print("Yes");
						PD::setColor(7);

					}
					else {

						PD::print("No");
						PD::setColor(7);

					}

					PD::setColor(7, 0);

				}

			}

			break;

		case Event_ViewState::SelectCard:

			for (uint8_t i = 0; i < 3; i++) {

				uint8_t renderIdx = Event_CardDrawOrder[(this->eventScreenVars.selection * 3) + i];

				renderLargeSpinningCardEvent(Event_ImageX[renderIdx], Event_ImageY[renderIdx], this->eventScreenVars.dice[renderIdx]);
				if (flash && this->eventScreenVars.selection == renderIdx) PD::drawBitmap(Event_ImageX[renderIdx], Event_ImageY[renderIdx], Images::Large_Card_Spinning_Highlight);

			}

			PD::setCursor(15, 0);
			printEventName(this->eventScreenVars.dice[this->eventScreenVars.selection]);

			break;


		case Event_ViewState::UpdateStats:

			renderLargeSpinningCardEvent(39, 14, this->eventScreenVars.dice[this->eventScreenVars.selection]);
			PD::setCursor(15, 0);
			printEventName(this->eventScreenVars.dice[this->eventScreenVars.selection]);

			break;

	}

	// Player statistics ..

	static const FlashSettings diceHelper[] =
	{
		FlashSettings::None,
		FlashSettings::FlashGold,
		FlashSettings::FlashHP,
		FlashSettings::FlashFood,
		FlashSettings::FlashXP,
		FlashSettings::FlashArmour,
	};

	const FlashSettings settings = (this->eventScreenVars.dice[this->eventScreenVars.selection] <= 5) ? static_cast<FlashSettings>(diceHelper[this->eventScreenVars.dice[this->eventScreenVars.selection]]) : FlashSettings::None;
	const bool shouldFlash = (this->eventScreenVars.viewState == Event_ViewState::UpdateStats && this->counter < FLASH_COUNTER);

	this->renderPlayerStatistics(shouldFlash, settings);

}

void Game::printEventName(uint8_t dice) {

	PD::setColor(7, 1);

	switch (dice - 1) {

		case 0:
			PD::print("Found Loot    +2 Gold");
			break;

		case 1:
			PD::print("Health Potion   +2 HP");
			break;

		case 2:
			PD::print("Found Meat    +1 Food");
			break;

		case 3:
			PD::print("Whetstone       +2 XP");
			break;

		case 4:
			PD::print("Found Shield    +1 AR");
			break;

		case 5:
			PD::print("  Monster       5 DMG");		
			break;

	}

}

void Game::renderLargeSpinningCardEvent(uint8_t x, uint8_t y, uint8_t dice) {

	this->renderLargeSpinningCard(x, y, 0);
	PD::drawBitmap(x + 2, y + 2, Images::Event_Dice[dice - 1]);

}