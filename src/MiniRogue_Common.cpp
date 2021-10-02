
#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;



// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::renderSpinningCard(int8_t x, int8_t y, uint8_t i) {

    const auto spinning_card = Images::Spinning_Card[i];
    PD::drawBitmap(x, y, spinning_card);

}

void Game::renderTreasureSpinningCard(int8_t x, int8_t y, uint8_t i) {

    const auto spinning_card = Images::Treasure_Spinning_Card[i];
    PD::drawBitmap(x, y, spinning_card);

}

void Game::renderLargeSpinningCard(int8_t x, int8_t y, uint8_t i) {

    const auto spinning_card = Images::Large_Spinning_Cards[i];
    PD::drawBitmap(x, y, spinning_card);

}

void Game::renderMessageBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {

    PD::setColor(0);
    PD::fillRect(x + 1, y, w - 2, h - 1);
    PD::fillRect(x, y + 1, w, h - 3);
    PD::setColor(9);
    PD::drawFastHLine(x + 3, y + 2, w - 6);
    PD::drawFastHLine(x + 3, y + h - 3, w - 6);
    PD::drawFastVLine(x + 2, y + 3, h - 7);
    PD::drawFastVLine(x + w - 3, y + 3, h - 7);

    PD::drawBitmap(x + 2, y + 2, Images::Dialogue_TL);
    PD::drawBitmap(x + w - 7, y + h - 7, Images::Dialogue_BR);
    PD::setColor(7);

}

void Game::renderPlayerDead(uint8_t y) {

    PD::drawBitmap(29, y, Images::Message_PlayerDied, 0);

}

void Game::renderMonsterDead() {

    renderMessageBox(25, 24, 60, 18);
    PD::setCursor(31, 30);
    PD::print("Enemy Killed");

}

void Game::renderBackground() {

    for (uint8_t y= 0; y < 88; y = y + 10)  {
        PD::drawBitmap(-2, y, Images::Background_Divider, NOROT, FLIPH);
        PD::drawBitmap(100, y, Images::Background_Divider);
    }

	PD::drawBitmap(13, 70, Images::Health);	

}

void Game::renderPlayerStatistics(bool overallFlash, FlashSettings settings) {

    const bool flash = (PC::frameCount % FLASH_DELAY) < (FLASH_DELAY / 2) && overallFlash;

    for (uint8_t x = 0; x < 6; x++) {

        uint8_t val = 0;
        bool flag = false;

        switch (x)  {

            case 0:   
                val = playerStats.xpTrack;       
                break;

            case 1:   
                val = playerStats.xp;       
                flag = ((settings & FlashSettings::FlashXP) != 0);
                break;

            case 2:   
                val = playerStats.hp;
                flag = ((settings & FlashSettings::FlashHP) != 0);
                break;

            case 3:   
                val = playerStats.armour;
                flag = ((settings & FlashSettings::FlashArmour) != 0);
                break;

            case 4:   
                val = playerStats.gold;
                flag = ((settings & FlashSettings::FlashGold) != 0);
                break;

            case 5:   
                val = playerStats.food;
                flag = ((settings & FlashSettings::FlashFood) != 0);
                break;

        }

        if (flag && flash) {
            PD::setColor(7);
            PD::fillRect(23 + ((x % 3) * 26), 70 + (x < 3 ? 0 : 9), (val < 10 ? 5 : 10), 6);
            PD::setColor(0, 7);
        }
        else {
            PD::setColor(7, 0);
        }

        PD::setCursor(24 + ((x % 3) * 26), 71 + (x < 3 ? 0 : 9));
        PD::print(static_cast<uint16_t>(val));
        PD::setColor(7, 0);

    }

    {

        uint8_t position = 0;

        for (uint8_t x = 0; x < 4; x ++ ) {

            if (this->playerStats.items[x] > 0) { drawItem(position, Images::Wands[x]); position++; }
            if (this->playerStats.items[x] > 1) { drawItem(position, Images::Wands[x]); position++; }

        }

        PD::setColor(6);
        PD::drawRect(89, 68, 8, 8);
        PD::drawRect(89, 78, 8, 8);
        PD::setColor(7);

    }

}

void Game::drawItem(uint8_t position, uint8_t const *imageName) {

    PD::drawBitmap(90, 69 + (position * 10), imageName);

}




/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 */
void Game::drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour) {

    uint8_t diff = (x2 - x1);

    for (uint8_t x = 0; x <= diff; x += 2) {

        PD::drawPixel(x1 + x, y, colour);

    }

}


/* ----------------------------------------------------------------------------
 *  Draw a vertical dotted line. 
 */
void Game::drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t colour) {

    uint8_t diff = (y2 - y1);

    for (uint8_t y = 0; y <= diff; y += 2) {

        PD::drawPixel(x, y1 + y, colour);

    }
  
}


void Game::resetGame() {

    this->gameStats.resetGame();
    this->playerStats.resetGame();

    for (uint8_t i = 0; i < 6; i++) { 
        this->cards[i] = static_cast<GameState>(i + 1); 
    }

    this->cards[6] = GameState::BossMonster_Init;

}

uint8_t Game::clamp(int16_t value, uint8_t low, uint8_t high) {

    return (value < low) ? low : (value > high) ? high : value; 

}