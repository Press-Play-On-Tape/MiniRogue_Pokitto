#include "MiniRogue.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::gameOver_Init() {
	printf("gameOver_Init start\n");
	this->gameOverScreenVars.init();
	this->counter = 0;

	switch (this->gameState) {

		case GameState::Winner:
			this->gameOverScreenVars.viewState = GameOver_ViewState::Winner; 
			break;

		default:
			this->gameOverScreenVars.viewState = GameOver_ViewState::HighScore; 
			break;

	}

	this->gameOverScreenVars.score = 0;
	this->gameOverScreenVars.score += (gameStats.skillLevel * 3);
	this->gameOverScreenVars.score += (playerStats.xpTrack * 2);
	this->gameOverScreenVars.score += ((gameStats.level + 1) * 3);
	this->gameOverScreenVars.score += (playerStats.hp * 2);
	this->gameOverScreenVars.score += (playerStats.food);
	this->gameOverScreenVars.score += (playerStats.armour);
	this->gameOverScreenVars.score += (playerStats.gold * 2);
	this->gameOverScreenVars.score += (playerStats.bossesKilled * 2);
	this->gameOverScreenVars.score += (playerStats.itemCount());

	this->gameOverScreenVars.highScore = this->cookie->highScore;

	if (this->gameOverScreenVars.score > this->gameOverScreenVars.highScore) {

		this->gameOverScreenVars.highScore = this->gameOverScreenVars.score;
		this->cookie->highScore = this->gameOverScreenVars.highScore;
		this->cookie->saveCookie();

	}

	this->gameState = GameState::GameOver;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::gameOver() { 

  if (PC::buttons.pressed(BTN_A)) { 
		
		switch (this->gameOverScreenVars.viewState) {

			case GameOver_ViewState::Winner: 
				this->gameOverScreenVars.viewState = GameOver_ViewState::HighScore; 
				break;

			case GameOver_ViewState::HighScore:
				this->gameState = GameState::TitleScreen; 
				break;

		}

	}


	// Clear scores ..

	if (PC::buttons.pressed(BTN_UP) && PC::buttons.pressed(BTN_DOWN)) {

		this->gameOverScreenVars.clearScores++;

		switch (this->gameOverScreenVars.clearScores) {

			case 21 ... 60:
				#ifdef USE_LEDS             
				arduboy.setRGBled(RED_LED, 128 - (this->gameOverScreenVars.clearScores * 2));
				#endif
				break;

			case 61:
				this->gameOverScreenVars.clearScores = 0;
				#ifdef USE_LEDS             
				arduboy.setRGBled(RED_LED, 0);
				#endif
				this->cookie->initialise();
				this->gameOverScreenVars.highScore = 0;
				this->gameOverScreenVars.score = 0;
				return;

		}

	}
	else {

		if (this->gameOverScreenVars.clearScores > 0) {
		
			#ifdef USE_LEDS             
			arduboy.setRGBled(RED_LED, 0);
			#endif
			this->gameOverScreenVars.clearScores = 0;

		}
		
	}


	// Render

	this->renderTitleBackground(false);

	switch (this->gameOverScreenVars.viewState) {

		case GameOver_ViewState::Winner:

			PD::drawBitmap(15, 19, Images::Winner);

			if (PC::frameCount % 70 < 7) {
				PD::drawBitmap(42, 4, Images::BlinkEyes_2);
			}
			else {
				PD::drawBitmap(42, 4, Images::BlinkEyes_1);
			}

			PD::drawFastHLine(8, 57, 94);
			PD::drawFastHLine(8, 59, 94);
			this->drawHorizontalDottedLine(8, 100, 61, 1);

			break;

		case GameOver_ViewState::HighScore:

			PD::drawBitmap(14, 4, Images::GameOverBanner, 0);
			
			PD::setCursor(17, 18);
			PD::print("Skill Level");
			PD::setCursor(72, 18);
			PD::print("x2");
			PD::setCursor(17, 27);
			PD::print("Area Reached");
			PD::setCursor(72, 27);
			PD::print("x3");
			PD::setCursor(17, 36);
			PD::print("Bosses Slayed");
			PD::setCursor(72, 36);
			PD::print("x2");
	
			PD::setCursor(86, 18);
			this->renderTwoDigitNumeric(gameStats.skillLevel * 2);
			PD::setCursor(86, 27);
			this->renderTwoDigitNumeric((gameStats.level + 1) * 3);
			PD::setCursor(86, 36);
			this->renderTwoDigitNumeric(playerStats.bossesKilled * 2);

			PD::drawBitmap(11, 47, Images::HighScore);

			PD::setCursor(28, 48);
			this->renderTwoDigitNumeric(playerStats.xpTrack * 2);
			PD::setCursor(28, 57);
			this->renderTwoDigitNumeric(playerStats.hp * 2);

			PD::setCursor(61, 48);
			this->renderTwoDigitNumeric(playerStats.gold * 2);
			PD::setCursor(61, 57);
			this->renderTwoDigitNumeric(playerStats.food);

			PD::setCursor(93, 48);
			this->renderTwoDigitNumeric(playerStats.armour);
			PD::setCursor(93, 57);
			this->renderTwoDigitNumeric(playerStats.itemCount());

			PD::setCursor(7, 71);
			PD::print("Score:");
			PD::setCursor(50, 71);
			PD::print("High");
			PD::setCursor(68, 71);
			PD::print("Score:");
			PD::setCursor(31, 71);
			this->renderThreeDigitNumeric(this->gameOverScreenVars.score);
			PD::setCursor(92, 71);
			this->renderThreeDigitNumeric(this->gameOverScreenVars.highScore);

			PD::drawFastHLine(7, 67, 96);
		
			break;

	}


}

void Game::renderTwoDigitNumeric(uint8_t val) { 

	if (val < 10) PD::print("0");
	PD::print(static_cast<uint16_t>(val));

}

void Game::renderThreeDigitNumeric(uint8_t val) { 

	if (val < 100) PD::print("0");
	if (val < 10) PD::print("0");

	PD::print(static_cast<uint16_t>(val));

}
