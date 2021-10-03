#pragma once

#include "Pokitto.h"
#include <LibAudio>
// #include "entities/Entities.h"
#include "utils/Structs.h"
#include "images/Images.h"
#include "utils/GameCookie.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

class Game {

    private:

        GameState gameState = GameState::SplashScreen_Init;
        // GameStateType gameStateType = GameStateType::Trap;

        GameCookie *cookie;

        SplashScreenVariables splashScreenVariables;
        GameStats gameStats;
        PlayerStats playerStats;

        EventScreenVariables eventScreenVars;
        FightMonsterScreenVariables fightMonsterScreenVars;
        MerchantScreenVariables merchantScreenVars;
        ShowCardsScreenVariables showCardsScreenVars;
        RestingScreenVariables restingScreenVars;
        TrapScreenVariables trapScreenVars;
        TreasureScreenVariables treasureScreenVars;
        GameOverScreenVariables gameOverScreenVars;

		uint8_t leftFlame = 0;
		uint8_t rightFlame = 1;
        uint8_t counter = 0;
        GameState cards[7];
        File mainThemeFile;
        uint8_t currentTheme = 255;

    public:

        void setup(GameCookie *cookie);
        void loop();

        void event_Init();
        void event();
        void fightMonster_Init();
        void fightMonster();
        void merchant_Init();
        void merchant();
        void splash_Init();
        void splash();
        void title_Init();
        void title();
        void showCards_Init();
        void showCards();
        void resting_Init();
        void resting();
        void trap_Init();
        void trap();
        void treasure_Init();
        void treasure();
        void gameOver_Init();
        void gameOver();




        // Utilities

        void resetGame();
        void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour) ;
        void drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t colour) ;
        void drawItem(uint8_t position, uint8_t const *imageName);
        void drawMonsterHead(uint8_t const *imageHead);
        uint8_t clamp(int16_t value, uint8_t low, uint8_t high);

        void renderPlayerStatistics(bool overallFlash, FlashSettings settings);
        void renderSpinningCard(int8_t x, int8_t y, uint8_t i);
        void renderTreasureSpinningCard(int8_t x, int8_t y, uint8_t i);
        void renderLargeSpinningCard(int8_t x, int8_t y, uint8_t i);
        void renderLargeSpinningCardEvent(uint8_t x, uint8_t y, uint8_t dice);
        void renderBackground();
        void renderPlayerDead(uint8_t y = 25);
        void renderSelectTreasure();
        void renderChestResults();
        void renderMessageBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
        void monsterIsDead();
        void renderFightMonster();
        void renderMonsterDead();

        uint8_t getMonsterDMG();
        void setDiceSelection(bool value);

        void printTrapName();
        void printCaption(uint8_t index);
        void printEventName(uint8_t dice);
        void renderTwoDigitNumeric(uint8_t val);
        void renderThreeDigitNumeric(uint8_t val); 

        void playTheme(uint8_t index);
        void muteTheme();
        void playSoundEffect(SoundEffect soundEffect);

        FightMonster_SelectedElement prevDiceSelection(FightMonster_SelectedElement element);
        FightMonster_SelectedElement nextDiceSelection(FightMonster_SelectedElement element);
        FightMonster_SelectedElement prevWandSelection(FightMonster_SelectedElement element);
        FightMonster_SelectedElement nextWandSelection(FightMonster_SelectedElement element);

};


