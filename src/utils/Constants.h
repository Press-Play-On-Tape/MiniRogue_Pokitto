#pragma once

#define SOUNDS
#define _DEBUG

#define COOKIE_INITIALISED 16


enum class GameState : uint8_t {
    None                    = 0,
    Resting_Init            = 1,
    Treasure_Init           = 2,
    Event_Init              = 3,
    Merchant_Init           = 4,
    Trap_Init               = 5,
    Monster_Init            = 6,
    BossMonster_Init        = 7,
    MonsterFromEvent_Init   = 8,
    GamePlay                = 9,
    ShowCards_Init          = 10,
    Winner_Init             = 11,
    GameOver_Init           = 12,
    SplashScreen_Init       = 13,
    SplashScreen            = 14,
    TitleScreen_Init        = 15,
    TitleScreen             = 16,    

    Resting                 = 17,
    ShowCards               = 18,
    Trap                    = 19,
    Treasure                = 20,
    Merchant                = 21,
    Event                   = 22,
    Monster                 = 23,
    BossMonster             = 24,
    MonsterFromEvent        = 25,
    GameOver                = 26,
    Winner                  = 27,

};

enum class Wand : uint8_t {
    Fire,
    Ice,
    Poison,
    Healing
};

enum class Event_ViewState : uint8_t {
    RollDice,
    SkillCheck,
    SkillCheckResult,
    SelectCard,
    UpdateStats,
};

enum class Merchant_ViewState : uint8_t {
    Buying,
    Selling,
};

enum class ShowCards_ViewState : uint8_t {
    DealCards,
    PlayCard,
    PlayerDead
};

enum class Resting_ViewState : uint8_t {
    SelectReward,
    UpdateStats,
};

enum class Resting_SelectedItem : uint8_t {
    Food,
    Heal,
    Weapon,
};

enum class Trap_ViewState : uint8_t {
    SkillCheck,
    SkillCheckResult,
    RollDice,
    UpdateStats,
    PlayerDead
};

enum class Treasure_ViewState : uint8_t {
    InitialRoll,
    RollDice,
    UpdateStats
};

enum class FightMonster_ViewState : uint8_t {
    HighlightMonsterStats,
    HighlightPlayerStats,
    RollDice,
    DiceSelection,
    WandSelection,
    Defend,
    MonsterDead,
    MonsterDead_Wait,
    PlayerDead,
    ItemIceUsed,
    ItemFireUsed,
    ItemPoisonUsed,
    ItemHealingUsed
};

enum class GameOver_ViewState : uint8_t {
    Winner,
    HighScore
};

enum class FightMonster_SelectedElement : int8_t {
    None = -1,
    ItemFire,
    ItemIce,
    ItemPoison,
    ItemHealing,
    Dice1,
    Dice2,
    Dice3,
    Dice4,
    Action,
};

enum FlashSettings : uint8_t {
	None = 0,
	FlashXP = (1 << 0),
	FlashHP = (1 << 1),
	FlashArmour = (1 << 2),
	FlashGold = (1 << 3),
	FlashFood = (1 << 4),
};

struct MonsterStats {
    uint8_t hp;
    uint8_t dmg;
};

enum class SoundEffect : uint8_t {
    EvilLaugh,
    RollDice,
    Positive,
    Negative,
};

enum class SoundEffects : uint8_t {
    Both,
    Music,
    SFX,
    None
};

inline SoundEffects &operator++(SoundEffects &c ) {
    c = static_cast<SoundEffects>( (static_cast<uint8_t>(c) + 1) % 4 );
    return c;
}

inline SoundEffects operator++(SoundEffects &c, int ) {
    SoundEffects result = c;
    ++c;
    return result;
}

inline SoundEffects &operator--(SoundEffects &c ) {
    if (static_cast<uint8_t>(c) > 0) {
        c = static_cast<SoundEffects>( (static_cast<uint8_t>(c) - 1) % 4 );
    }
    else {
        c = static_cast<SoundEffects>( 3 );
    }
    return c;
}

inline SoundEffects operator--(SoundEffects &c, int ) {
    SoundEffects result = c;
    --c;
    return result;
}

constexpr const static uint8_t FLASH_DELAY = 16;
constexpr const static uint8_t FLASH_COUNTER = 70;

constexpr const static uint8_t MIN_LEVEL = 0;
constexpr const static uint8_t MAX_LEVEL = 2;
constexpr const static uint8_t UPLOAD_DELAY = 16;

constexpr const static uint8_t WINNER_LEVEL = 5;
constexpr const static uint8_t DO_NOT_EDIT_SLOT = 255;
constexpr const static uint8_t MAX_NUMBER_OF_SCORES         = 5;

const uint8_t InitSettings[] = {
    1, 5, 5, 6,
    0, 5, 3, 6,
    0, 4, 2, 5,
    0, 3, 1, 3
};

const uint8_t DiceDelay[] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 4, 4, 4, 8, 8,
};

