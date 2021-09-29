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
    uint8_t skillCheck = 0;
    uint8_t dice[3] = { 0, 0, 0 };
    uint8_t selection = 0;

    void init() {

        this->viewState = Event_ViewState::SkillCheck;
        this->nextState = Event_ViewState::SkillCheck;
        this->skillCheck = 0;
        this->dice[0] = 0;
        this->dice[1] = 0;
        this->dice[2] = 0;
        this->selection = 0;

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

        this->viewState;
        this->selectedItem = Resting_SelectedItem::Food;
        this->prevSelection = Resting_SelectedItem::Food;

    }

};

struct TrapScreenVariables {

    Trap_ViewState viewState;
    uint8_t dice;

    void init() {

        this->viewState = Trap_ViewState::SkillCheck;
        this->dice = 0;

    }

};

struct TreasureScreenVariables {

    Treasure_ViewState viewState = Treasure_ViewState::InitialRoll;
    uint8_t dice = 0;
    bool foundTreasure = false;

    void init() {

        this->viewState = Treasure_ViewState::InitialRoll;
        this->dice = 0;
        this->foundTreasure = false;

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
    uint8_t xpTrack = 1;
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

        static const uint8_t PROGMEM xpLevels[] = {0, 6, 12, 18, 99};

        uint8_t xpLevel = pgm_read_byte(&xpLevels[xpTrack]);

        xp = xp + value;

        if (xp > xpLevel) {

            xp = xp - (xpLevel - 1);
            xpTrack++;

        }

    }

    void resetGame() {

        for (uint8_t x = 0; x < 4; x++) { items[x] = 0; }
        xpTrack = 1;
        xp = 0;
        bossesKilled = 0;

    }

    uint8_t itemCount() {

        return items[0] + items[1] + items[2] + items[3];

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

        static const uint8_t drops[] PROGMEM = { 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, };
        level = (level < 10) ? (level + pgm_read_byte(&drops[level])) : level;

    }

    uint8_t getAreaId() {

        static const uint8_t ids[] PROGMEM = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, };
        return (level < 15) ? pgm_read_byte(&ids[level]) : 5;

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

        room++;

        switch (room) {

            case 1:   selectedCard = 0;   break;
            case 2:   selectedCard = 1;   break;
            case 3:   selectedCard = 3;   break;
            case 4:   selectedCard = 4;   break;
            case 5:   selectedCard = 6;   break;

        }

        if ((room == 6) && (level == 13)) {

            return GameState::Winner;

        }
        else {

            if (room == 6 || (room == 5 && !isLastLevelInArea())) {

                playerStats.decFood(1);
                room = 0;

                if (playerStats.food >= 0) {

                    level++;
                    selectedCard = 0;
                    monsterDefeated = false;

                }

            }

            return GameState::ShowCards_Init;

        }

    }


    uint8_t getMonsterDMG() {

        return (getAreaId() + 1) * 2;

    }


    uint8_t getBossMonsterDMG() {

        static const uint8_t damage[] PROGMEM = { 3, 5, 7, 9, 12, };

        auto areaId = getAreaId();
        return (areaId < 5) ? pgm_read_byte(&damage[areaId]) : 0;

    }

    uint8_t getMonsterReward() {

        return (getAreaId() + 1);

    }

};

