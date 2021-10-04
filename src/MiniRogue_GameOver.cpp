#include "MiniRogue.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::gameOver_Init() {

	this->gameOverScreenVars.init();
	this->counter = 0;

	switch (this->gameState) {

		case GameState::Winner_Init:
			this->gameOverScreenVars.viewState = GameOver_ViewState::Winner; 
			break;

		default:
			this->gameOverScreenVars.viewState = GameOver_ViewState::HighScore; 
			break;

	}

	this->gameOverScreenVars.score = 0;
	this->gameOverScreenVars.score += (gameStats.skillLevel * 3);
	this->gameOverScreenVars.score += (playerStats.getXPTrack() * 2);
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
				this->gameState = GameState::TitleScreen_Init; 
				break;

		}

	}


	// Clear scores ..

	if (PC::buttons.pressed(BTN_UP) && PC::buttons.pressed(BTN_DOWN)) {

		this->gameOverScreenVars.clearScores++;

		switch (this->gameOverScreenVars.clearScores) {

			case 21 ... 60:
				break;

			case 61:
				this->gameOverScreenVars.clearScores = 0;
				this->cookie->initialise();
				this->gameOverScreenVars.highScore = 0;
				this->gameOverScreenVars.score = 0;
				return;

		}

	}
	else {

		if (this->gameOverScreenVars.clearScores > 0) {
		
			this->gameOverScreenVars.clearScores = 0;

		}
		
	}


	// Render

    PD::drawBitmap(-9, 0, Images::GameOverSide);
    PD::drawBitmap(99, 0, Images::GameOverSide, NOROT, FLIPH);

    PD::drawBitmap(-4, 25, Images::Flames[this->leftFlame]);
    PD::drawBitmap(98, 25, Images::Flames[this->rightFlame], NOROT, FLIPH);

    if (PC::frameCount % 3 == 0) { 
        this->leftFlame = (this->leftFlame + 1) % 4;
        this->rightFlame = (this->rightFlame + 1) % 4;
    }

	switch (this->gameOverScreenVars.viewState) {

		case GameOver_ViewState::Winner:

			PD::drawBitmap(15, 19, Images::Winner);

			if (PC::frameCount % 70 < 7) {
				PD::drawBitmap(39, 4, Images::BlinkEyes_2);
				PD::drawBitmap(61, 4, Images::BlinkEyes_2, NOROT, FLIPH);
			}
			else {
				PD::drawBitmap(39, 4, Images::BlinkEyes_1);
				PD::drawBitmap(61, 4, Images::BlinkEyes_1, NOROT, FLIPH);
			}

			PD::setColor(6);
			PD::drawFastHLine(8, 60, 94);
			PD::setColor(5);
			PD::drawFastHLine(8, 62, 94);
			this->drawHorizontalDottedLine(8, 100, 64, 5);
			FlashSettings flashSettings;
			PD::drawBitmap(13, 70, Images::Health);	
			this->renderPlayerStatistics(false, flashSettings);

			break;

		case GameOver_ViewState::HighScore:

			PD::drawBitmap(15, 4, Images::GameOverBanner, 0);

			PD::setCursor(17, 18);
			PD::print("Skill Level");
			PD::setCursor(72, 18);
			PD::print("x2");
			PD::setCursor(17, 28);
			PD::print("Area Reached");
			PD::setCursor(72, 28);
			PD::print("x3");
			PD::setCursor(17, 38);
			PD::print("Bosses Slayed");
			PD::setCursor(72, 38);
			PD::print("x2");
	
			PD::setCursor(86, 18);
			this->renderTwoDigitNumeric(gameStats.skillLevel * 2);
			PD::setCursor(86, 28);
			this->renderTwoDigitNumeric((gameStats.level + 1) * 3);
			PD::setCursor(86, 38);
			this->renderTwoDigitNumeric(playerStats.bossesKilled * 2);

			PD::drawBitmap(11, 53, Images::HighScore);

			PD::setCursor(28, 54);
			this->renderTwoDigitNumeric(playerStats.getXPTrack() * 2);
			PD::setCursor(28, 64);
			this->renderTwoDigitNumeric(playerStats.hp * 2);

			PD::setCursor(61, 54);
			this->renderTwoDigitNumeric(playerStats.gold * 2);
			PD::setCursor(61, 64);
			this->renderTwoDigitNumeric(playerStats.food);

			PD::setCursor(93, 54);
			this->renderTwoDigitNumeric(playerStats.armour);
			PD::setCursor(93, 64);
			this->renderTwoDigitNumeric(playerStats.itemCount());

			PD::setCursor(7, 80);
			PD::print("Score:");
			PD::setCursor(50, 80);
			PD::print("High");
			PD::setCursor(68, 80);
			PD::print("Score:");
			PD::setCursor(31, 80);
			this->renderThreeDigitNumeric(this->gameOverScreenVars.score);
			PD::setCursor(92, 80);
			this->renderThreeDigitNumeric(this->gameOverScreenVars.highScore);

			PD::setColor(5);
			PD::drawFastHLine(7, 77, 96);
			PD::setColor(7);

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
