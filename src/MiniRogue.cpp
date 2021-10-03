#include "MiniRogue.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


//------------------------------------------------------------------------------
//  Setup 
//------------------------------------------------------------------------------

void Game::setup(GameCookie *cookie) {

    this->cookie = cookie;
    this->gameState = GameState::SplashScreen_Init;

}


void Game::loop() {

    PD::clear();
    PC::buttons.pollButtons();

    switch (this->gameState) {

        case GameState::Event_Init:
            this->event_Init();
            [[fallthrough]]

        case GameState::Event:
            this->event();
            break;

        case GameState::Monster_Init:
        case GameState::BossMonster_Init:
        case GameState::MonsterFromEvent_Init:
            this->fightMonster_Init();
            [[fallthrough]]

        case GameState::Monster:
        case GameState::BossMonster:
        case GameState::MonsterFromEvent:
            this->fightMonster();
            break;

        case GameState::Merchant_Init:
            this->merchant_Init();
            [[fallthrough]]

        case GameState::Merchant:
            this->merchant();
            break;

        case GameState::SplashScreen_Init:
            this->splash_Init();
            [[fallthrough]]

        case GameState::SplashScreen:
            this->splash();
            break;

        case GameState::TitleScreen_Init:
            this->title_Init();
            [[fallthrough]]

        case GameState::TitleScreen:
            this->title();
            break;

        case GameState::ShowCards_Init:
            this->showCards_Init();
            [[fallthrough]]

        case GameState::ShowCards:
            this->showCards();
            break;

        case GameState::Resting_Init:
            this->resting_Init();
            [[fallthrough]]

        case GameState::Resting:
            this->resting();
            break;

        case GameState::Trap_Init:
            this->trap_Init();
            [[fallthrough]]

        case GameState::Trap:
            this->trap();
            break;

        case GameState::Treasure_Init:
            this->treasure_Init();
            [[fallthrough]]

        case GameState::Treasure:
            this->treasure();
            break;

        case GameState::GameOver_Init:
        case GameState::Winner_Init:
            this->gameOver_Init();
            [[fallthrough]]

        case GameState::GameOver:
        case GameState::Winner:
            this->gameOver();
            break;
            
    }

}

