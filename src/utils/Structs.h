#pragma once

#include "Constants.h"

struct SplashScreenVariables {

    uint8_t counter = 0;

    void init() {

        this->counter = 0;

    }

};

struct EventScreenVariables {

    Event_ViewState viewState = Event_ViewState::SkillCheck;
    Event_ViewState nextState = Event_ViewState::SkillCheck;
    uint8_t skillCheck[6];
    uint8_t dice[3] = { 0, 0, 0 };
    uint8_t selection = 0;
    bool hasSkill = false;

    void init() {

        this->viewState = Event_ViewState::SkillCheck;
        this->nextState = Event_ViewState::SkillCheck;
        this->dice[0] = 0;
        this->dice[1] = 0;
        this->dice[2] = 0;
        this->selection = 0;
        this->hasSkill = false;

        for (uint8_t i = 0; i < 6; i++) {
            this->skillCheck[i] = 0;
        }

    }

};

struct FightMonsterScreenVariables {

    FightMonster_ViewState viewState = FightMonster_ViewState::RollDice;
    FightMonster_ViewState nextState = FightMonster_ViewState::RollDice;
    FightMonster_ViewState lastState = FightMonster_ViewState::RollDice;
    MonsterStats monsterStats;
    FightMonster_SelectedElement selectedElement = FightMonster_SelectedElement::None;

    uint8_t dice[4];
    uint8_t dice_Sixes[4];
    uint8_t diceMonster = 0;
    uint8_t ice = 0;
    uint8_t monsterPositionInc = 0;
    uint8_t monsterPosition = 0;

    bool dice_Retain[4];
    bool poison = false;

    void init() {

        this->viewState = FightMonster_ViewState::RollDice;
        this->nextState = FightMonster_ViewState::RollDice;
        this->lastState = FightMonster_ViewState::RollDice;

        this->selectedElement = FightMonster_SelectedElement::None;

        for (uint8_t i = 0; i < 4; i++) {

            this->dice[i] = 0;
            this->dice_Sixes[i] = 0;
            this->dice_Retain[i] = false;

        }

        this->diceMonster = 0;
        this->ice = 0;
        this->poison = false;

    }

};

struct MerchantScreenVariables {

    Merchant_ViewState viewState;
    uint8_t selectedItem = 0;
    uint8_t errorNumber = 0;
    FlashSettings settings;

    void init() {

        this->viewState = Merchant_ViewState::Buying;
        this->selectedItem = 0;
        this->errorNumber = 0;
        this->settings = FlashSettings::None;

    }

};

struct ShowCardsScreenVariables {

    ShowCards_ViewState viewState;
    uint8_t numberOfCardsToDisplay = 6;
    int8_t displayCard = 3;

    void init() {

        this->viewState = ShowCards_ViewState::DealCards;
        this->numberOfCardsToDisplay = 6;
        this->displayCard = 3;
 
    }

};

struct RestingScreenVariables {

    Resting_ViewState viewState;
    Resting_SelectedItem selectedItem = Resting_SelectedItem::Food;
    Resting_SelectedItem prevSelection = Resting_SelectedItem::Food;

    void init() {

        this->viewState = Resting_ViewState::SelectReward;
        this->selectedItem = Resting_SelectedItem::Food;
        this->prevSelection = Resting_SelectedItem::Food;

    }

};

struct TrapScreenVariables {

    Trap_ViewState viewState;
    uint8_t dice;
    uint8_t skillCheck[6];
    bool hasSkill = false;    

    void init() {

        this->viewState = Trap_ViewState::SkillCheck;
        this->dice = 0;
        this->hasSkill = false;    

        for (uint8_t i = 0; i < 6; i++) {
            this->skillCheck[i] = 0;
        }

    }

};

struct TreasureScreenVariables {

    Treasure_ViewState viewState = Treasure_ViewState::InitialRoll;
    uint8_t dice = 0;
    uint8_t skillCheck[6];
    bool hasSkill = false;    

    void init() {

        this->viewState = Treasure_ViewState::InitialRoll;
        this->dice = 0;
        this->hasSkill = false;    

        for (uint8_t i = 0; i < 6; i++) {
            this->skillCheck[i] = 0;
        }

    }

};

struct GameOverScreenVariables {

    GameOver_ViewState viewState = GameOver_ViewState::Winner;
    uint8_t score = 0;
    uint8_t highScore = 0;
    uint8_t clearScores = 0;

    void init() {

        this->viewState = GameOver_ViewState::Winner;
        this->score = 0;
        this->highScore = 0;
        this->clearScores = 0;

    }

};

struct PlayerStats {

    uint8_t armour;
    int8_t food;
    uint8_t gold;
    uint8_t hp;
    uint8_t xp;
    uint8_t bossesKilled;
    uint8_t items[4];

    void decArmour(uint8_t val) {

        armour = max(armour - val, 0);

    }

    void incArmour(uint8_t val) {

        armour = min(armour + val, 5);

    }

    void decFood(uint8_t val) {

        food = food - val;

    }

    void incFood(uint8_t val) {

        food = min(food + val, 10);

    }

    void decGold(uint8_t val) {

        gold = max(gold - val, 0);

    }

    void incGold(uint8_t val) {

        gold = min(gold + val, 10);

    }

    void decHP(uint8_t val) {

        hp = max(hp - val, 0);
    
    }

    void incHP(uint8_t val) {

       hp = min(hp + val, 20);
 
    }

    void incXP(uint8_t value) {

        this->xp = xp + value;

    }

    uint8_t getXPTrack() {

        switch (this->xp) {

            case 0 ... 5:   return 1;
            case 6 ... 17:  return 2;
            default:        return 3;

        }
        
    }

    void resetGame() {

        for (uint8_t x = 0; x < 4; x++) { this->items[x] = 0; }
        this->xp = 0;
        this->bossesKilled = 0;

    }

    uint8_t itemCount() {

        return this->items[0] + this->items[1] + this->items[2] + this->items[3];

    }

};

struct GameStats {

    uint8_t skillLevel = 1;
    uint8_t level = 1;
    uint8_t room = 0;
    uint8_t selectedCard = 0;
    bool monsterDefeated = false;

    void resetGame() {

        level = 0;
        room = 0;
        monsterDefeated = false;
        selectedCard = 0;

    }

    void dropArea() {

        static const uint8_t drops[] = { 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, };
        level = (level < 10) ? (level + drops[level]) : level;

    }

    uint8_t getAreaId() {

        static const uint8_t ids[] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, };
        return (level < 15) ? ids[level] : 5;

    }

    bool isLastLevelInArea() {

        switch (level) {

            case 1:
            case 3:
            case 6:
            case 9:
            case 13:
                return true;

            default:
                return false;

        }

    }

    GameState incRoom(PlayerStats & playerStats) {

        this->room++;

        switch (this->room) {

            case 1:   this->selectedCard = 0;   break;
            case 2:   this->selectedCard = 1;   break;
            case 3:   this->selectedCard = 3;   break;
            case 4:   this->selectedCard = 4;   break;
            case 5:   this->selectedCard = 6;   break;

        }

        if ((this->room == 6) && (this->level == 13)) {

            return GameState::Winner_Init;

        }
        else {

            if (this->room == 6 || (this->room == 5 && !isLastLevelInArea())) {

                playerStats.decFood(1);
                this->room = 0;

                if (playerStats.food >= 0) {
                    
                    this->level++;
                    this->selectedCard = 0;
                    this->monsterDefeated = false;

                }

            }

            return GameState::ShowCards_Init;

        }

    }


    uint8_t getMonsterDMG() {

        return (this->getAreaId() + 1) * 2;

    }


    uint8_t getBossMonsterDMG() {

        static const uint8_t damage[] = { 3, 5, 7, 9, 12, };

        auto areaId = this->getAreaId();
        return (areaId < 5) ? damage[areaId] : 0;

    }

    uint8_t getMonsterReward() {

        return (this->getAreaId() + 1);

    }

};

