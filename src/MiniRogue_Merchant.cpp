#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::merchant_Init() {

	this->merchantScreenVars.init();
	this->counter = 0;
	this->gameState = GameState::Merchant;

	this->playSoundEffect(SoundEffect::Welcome);
  
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::merchant() { 

	if (PC::buttons.pressed(BTN_A) || PC::buttons.pressed(BTN_B) || PC::buttons.pressed(BTN_UP) ||PC::buttons.pressed(BTN_DOWN) || PC::buttons.pressed(BTN_LEFT) ||PC::buttons.pressed(BTN_RIGHT) ) this->merchantScreenVars.errorNumber = 0;

	switch (this->merchantScreenVars.viewState) {

		case Merchant_ViewState::Buying:

			if (PC::buttons.pressed(BTN_UP) && this->merchantScreenVars.selectedItem > 0)            this->merchantScreenVars.selectedItem--;
			if (PC::buttons.pressed(BTN_DOWN) && this->merchantScreenVars.selectedItem < 6)          this->merchantScreenVars.selectedItem++;

			if (PC::buttons.pressed(BTN_A)) {

				switch (this->merchantScreenVars.selectedItem) {

					case 0:

						if (playerStats.gold == 0) {
							this->merchantScreenVars.errorNumber = 1;
						} 
						else if (playerStats.food == 10) { 
							this->merchantScreenVars.errorNumber = 3;
						} 
						else {
							this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashFood));
							this->playerStats.incFood(1); 
							this->playerStats.decGold(1); 
						}   

						break;

					case 1:

						if (playerStats.gold == 0) {
							this->merchantScreenVars.errorNumber = 1;
						} 
						else if (playerStats.hp == 20) { 
							this->merchantScreenVars.errorNumber = 3;
						} 
						else { 
							this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashHP));
							this->playerStats.incHP(1); 
							this->playerStats.decGold(1);
						}   

						break;

					case 2:

						if (playerStats.gold < 3) {
							this->merchantScreenVars.errorNumber = 1;
						} 
						else if (playerStats.hp == 20) { 
							this->merchantScreenVars.errorNumber = 3;
						} 
						else { 
							this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashHP));
							this->playerStats.incHP(4); 
							this->playerStats.decGold(3); 
						}   

						break;

					case 3 ... 5:

						if (playerStats.gold >= 8) {

							if (playerStats.itemCount() < 2) { 

								this->playerStats.items[this->merchantScreenVars.selectedItem - 3]++; 
								this->playerStats.decGold(8); 
								this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashGold));

							}
							else {
								this->merchantScreenVars.errorNumber = 3;
							}

						}
						else {
							this->merchantScreenVars.errorNumber = 1;
						}   

						break;

					case 6:

						if (playerStats.gold >= 6) {

							if (playerStats.armour < 5) { 

								this->playerStats.incArmour(1); 
								this->playerStats.decGold(6); 
								this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashArmour));

							}
							else {
								this->merchantScreenVars.errorNumber = 3;
							}

						}
						else {
							this->merchantScreenVars.errorNumber = 1;
						}   

						break;

				}

				constexpr FlashSettings flashTest = static_cast<FlashSettings>(static_cast<uint8_t>(FlashSettings::FlashHP) | static_cast<uint8_t>(FlashSettings::FlashXP) | static_cast<uint8_t>(FlashSettings::FlashFood) | static_cast<uint8_t>(FlashSettings::FlashArmour));

				if ((this->merchantScreenVars.settings & flashTest) != FlashSettings::None)
				this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashGold));

			}

			break;

		case Merchant_ViewState::Selling:

			if (PC::buttons.pressed(BTN_UP) && this->merchantScreenVars.selectedItem > 0)            this->merchantScreenVars.selectedItem--;
			if (PC::buttons.pressed(BTN_DOWN) && this->merchantScreenVars.selectedItem < 3)          this->merchantScreenVars.selectedItem++;

			if (PC::buttons.pressed(BTN_A)) {

				if (this->merchantScreenVars.selectedItem <= 2) {

					if (playerStats.items[this->merchantScreenVars.selectedItem] > 0) {

						this->playerStats.items[this->merchantScreenVars.selectedItem]--;
						this->playerStats.incGold(4);
						this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashGold));

					}
					else {
						this->merchantScreenVars.errorNumber = 2;
					}

				}
				else {

					if (playerStats.armour > 0) {

						this->playerStats.decArmour(1);
						this->playerStats.incGold(3);
						this->merchantScreenVars.settings = static_cast<FlashSettings>(static_cast<uint8_t>(this->merchantScreenVars.settings) | static_cast<uint8_t>(FlashSettings::FlashArmour));

					}
					else {
						this->merchantScreenVars.errorNumber = 2;
					}

				}

			} 

			break;

	}

	if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.pressed(BTN_RIGHT)) {

		this->merchantScreenVars.viewState = static_cast<Merchant_ViewState>(!static_cast<bool>(this->merchantScreenVars.viewState));
		this->merchantScreenVars.selectedItem = 0;

	}

	if (PC::buttons.pressed(BTN_B)) {

		uint8_t oldArea = this->gameStats.getAreaId();
		this->gameState = this->gameStats.incRoom(playerStats); 

		if (oldArea != this->gameStats.getAreaId()) {

			this->playTheme(this->gameStats.getAreaId());
			
		}

	}


	// Render

	this->renderBackground();
	PD::drawBitmap(50, 2, Images::Merchant_Only);
    PD::drawBitmap(93, 37, Images::Merchant_Flames[this->rightFlame]);

    if (PC::frameCount % 3 == 0) { 
        this->rightFlame = (this->rightFlame + 1) % 4;
    }

	PD::setColor(7);

	if (PC::frameCount % 50 < 4) { 
		PD::drawFastHLine(69, 13, 7); 
	}


	{

		uint8_t const *imageName = nullptr;

		switch (this->merchantScreenVars.viewState) {

			case Merchant_ViewState::Buying:
				imageName = Images::Merchant_Buying;
				break;

			case Merchant_ViewState::Selling:
				imageName = Images::Merchant_Sell;
				break;

		}

		PD::drawBitmap(13, 0, imageName);

	}


	// Highlight ..

	PD::drawFastVLine(13, 8 + (this->merchantScreenVars.selectedItem * 8), 5);
	PD::drawFastVLine(44, 8 + (this->merchantScreenVars.selectedItem * 8), 5);


	// Error Message ?

	if (this->merchantScreenVars.errorNumber > 0) {

		this->renderMessageBox(22, 23, 64, 26);
		PD::setColor(7);

		switch (this->merchantScreenVars.errorNumber - 1) {

			case 0:
				PD::setCursor(27, 29);
				PD::print("    You need");
				PD::setCursor(27, 37);
				PD::print("   more gold!");
				break;

			case 1:
				PD::setCursor(27, 29);
				PD::print(" You dont have");
				PD::setCursor(27, 37);
				PD::print(" one to sell!");
				break;

			case 2:
				PD::setCursor(27, 29);
				PD::print("  You already");
				PD::setCursor(27, 37);
				PD::print(" have enough!");
				break;

		}

	}

	this->renderPlayerStatistics(true, this->merchantScreenVars.settings);

}
