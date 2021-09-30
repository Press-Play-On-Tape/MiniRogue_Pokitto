#include "MiniRogue.h"

constexpr const static int8_t STARVED_TO_DEATH = -1; 
constexpr const static uint8_t ShowCards_NumberOfCardsInFlip = 7; 
constexpr const static uint8_t DEAL_DELAY = 5; 
constexpr const static uint8_t CARD_SHOW_NONE = -1;
constexpr const static uint8_t CARD_SHOW_ALL = 127;
constexpr const static uint8_t CARD_NONE_SELECTED = 255;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::showCards_Init() {

	this->showCardsScreenVars.init();
	this->showCardsScreenVars.numberOfCardsToDisplay = (gameStats.isLastLevelInArea() ? 7 : 6);
	this->counter = 0;           

	if (gameStats.room <= 0) {

		this->showCardsScreenVars.viewState = ShowCards_ViewState::DealCards;
		this->showCardsScreenVars.displayCard = -3;
		this->counter = ShowCards_NumberOfCardsInFlip;


		// Shuffle cards ..

		this->cards[6] = GameState::BossMonster_Init;
		
		for (uint8_t i = 0; i < 10; i++) { 
			
			uint8_t x = random(0, 6);
			uint8_t y = random(0, 6);
			swap(this->cards[x], this->cards[y]);

		}

		if (this->playerStats.food == STARVED_TO_DEATH) {

  			this->showCardsScreenVars.displayCard = CARD_SHOW_ALL;
			this->showCardsScreenVars.viewState = ShowCards_ViewState::PlayerDead;
			playerStats.food = 0;
			gameStats.room = 0;

		}
		else {

			gameStats.room = 1;
			
		}

	}
	else {

		this->showCardsScreenVars.displayCard = CARD_SHOW_ALL;
  		this->counter = ShowCards_NumberOfCardsInFlip;
		this->showCardsScreenVars.viewState = ShowCards_ViewState::PlayCard;
		
	}

	this->gameState = GameState::ShowCards; 

// this->cards[0] = GameState::Monster_Init;//SJH SJH SJH
// this->playerStats.hp = 1;//SJH
// this->playerStats.items[3]++;


}

void Game::showCards() {

	const int8_t  cardPositionX[] = { 5, 25, 25, 45, 65, 65, 85 };
	const uint8_t cardPositionY[] = { 23, 8, 36, 23, 8, 36, 23 };
	const uint8_t cardIndexToRoom[] = { 1, 2, 2, 3, 4, 4, 5 };

	switch (this->showCardsScreenVars.viewState) {

		case ShowCards_ViewState::DealCards:

			if (PC::frameCount % DEAL_DELAY == 0) {

				if (this->showCardsScreenVars.displayCard < this->showCardsScreenVars.numberOfCardsToDisplay) {
					this->showCardsScreenVars.displayCard++;
				}
				else {
					this->showCardsScreenVars.displayCard = CARD_SHOW_ALL;
		        	this->showCardsScreenVars.viewState = ShowCards_ViewState::PlayCard;
					this->counter = ShowCards_NumberOfCardsInFlip;
				}

			}
			break;

		case ShowCards_ViewState::PlayCard:

			if (PC::buttons.pressed(BTN_UP) && (gameStats.selectedCard == 2 || gameStats.selectedCard == 5)) 	{ gameStats.selectedCard--;}
			if (PC::buttons.pressed(BTN_DOWN) && (gameStats.selectedCard == 1 || gameStats.selectedCard == 4)) 	{ gameStats.selectedCard ++; }

			if (PC::buttons.pressed(BTN_A)) { 
		
				this->gameState = this->cards[gameStats.selectedCard]; 
				
			}

			break;

		case ShowCards_ViewState::PlayerDead:

      		if (PC::buttons.pressed(BTN_A)) { 
        		this->gameState = GameState::GameOver_Init;
			}

			break;

	}



	// Render screen

	
	uint8_t room = gameStats.room;

	// Display area names ..

	PD::setColor(7);
	PD::setCursor(11, 0);

	PD::print("L");
	PD::print(gameStats.getAreaId() + 1);
	PD::print(" A");
	PD::print(gameStats.level + 1);

	switch (gameStats.getAreaId()) {

		case 0: 
			PD::print(": The Black Sewers");
			break;

		case 1: 
			PD::print(" Poisonous Dungeons");
			break;

		case 2: 
			PD::print(" Undead Catacombs");
			break;

		case 3: 
			PD::print(": Flaming Underworld");
			break;

		case 4: 
			PD::print(": Sunken Keep of Og!");
			break;
			
	}


	// Draw background ..

	PD::drawBitmap(13, 70, Images::Health);	
 	PD::drawBitmap(0, 18, Images::Card_Border);


	// Player statistics ..

	const FlashSettings settings = ((showCardsScreenVars.viewState == ShowCards_ViewState::PlayerDead) ? FlashSettings::FlashFood : FlashSettings::None);

	this->renderPlayerStatistics(true, settings);

	const bool flash = (PC::frameCount % 36) < 18;

	for (uint8_t i = 0; i < 7; i++) {

		if (this->showCardsScreenVars.displayCard >= i) {

			int8_t x =  cardPositionX[i];
			uint8_t y = cardPositionY[i];
			uint8_t r = cardIndexToRoom[i];


      		// Draw cards as they are being dealt ..

			if (this->counter == ShowCards_NumberOfCardsInFlip || (room != r && this->counter > 0) || this->counter == 0) {

        		this->renderSpinningCard(x, y, 6);

			}


      		// Draw card highlight ..
      
			if (this->showCardsScreenVars.displayCard == CARD_SHOW_ALL && (gameStats.selectedCard == i) && flash && room != 0) {

        		PD::drawBitmap(x, y, Images::Card_Highlight);

			}

			if (room != 0) {

				if ((room > r) || (room == r && this->counter == 0) || (i == 6 && this->showCardsScreenVars.numberOfCardsToDisplay == 6)) {

					PD::setColor(0);
					PD::fillRect(x + 2, y + 2, 17, 24);

					PD::drawBitmap(x + 3, y + 6, Images::Card_Faces[(i == 6 && this->showCardsScreenVars.numberOfCardsToDisplay == 6 ? 7 : static_cast<uint8_t>(this->cards[i]) - 1)]);

				}

				if (room == r && this->counter > 0 && this->showCardsScreenVars.displayCard == CARD_SHOW_ALL) {

					this->renderSpinningCard(x, y, this->counter - 1);

				}

			}

		}

	}

	if (this->counter > 0 && this->showCardsScreenVars.displayCard == CARD_SHOW_ALL) this->counter--;


	// Are we dead?

	if (showCardsScreenVars.viewState == ShowCards_ViewState::PlayerDead) {

		this->renderPlayerDead();

	}

}

