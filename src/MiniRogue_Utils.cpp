#include "MiniRogue.h"
#include "sounds/Sounds.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


void Game::playTheme(uint8_t index) {

    #ifdef SOUNDS

    constexpr char themes[5][19] = { "music/MiniR_00.raw", "music/MiniR_01.raw", "music/MiniR_02.raw", "music/MiniR_03.raw", "music/MiniR_04.raw" };

    switch (this->cookie->sfx) {

        case SoundEffects::Music:
        case SoundEffects::Both:

            if (this->mainThemeFile.openRO(themes[index % 5])) {
                auto& music = Audio::play<0>(this->mainThemeFile);
                music.setLoop(true);
                this->currentTheme = index;
            }

            break;

        case SoundEffects::SFX:
        case SoundEffects::None:

            this->muteTheme();

    }

    #endif

}

void Game::muteTheme() {

    #ifdef SOUNDS
    Audio::stop<0>();
    #endif

}


void Game::playSoundEffect(SoundEffect soundEffect) {

    #ifdef SOUNDS

    switch (this->cookie->sfx) {

        case SoundEffects::SFX:
        case SoundEffects::Both:
                
            switch (soundEffect) {
                
                case SoundEffect::EvilLaugh: 
                    Audio::play<1>(Sounds::sfx_EvilLaugh);    
                    break;
                
                case SoundEffect::RollDice: 
                    Audio::play<1>(Sounds::sfx_RollDice);    
                    break;
                
                case SoundEffect::Positive: 
                    Audio::play<1>(Sounds::sfx_Positive);    
                    break;
                
                case SoundEffect::Negative: 
                    Audio::play<1>(Sounds::sfx_Negative);    
                    break;
                
                case SoundEffect::FistPunch1: 
                    Audio::play<1>(Sounds::sfx_FistPunch1);    
                    break;
                
                case SoundEffect::FistPunch2: 
                    Audio::play<1>(Sounds::sfx_FistPunch2);    
                    break;
                
                case SoundEffect::Welcome: 
                    Audio::play<1>(Sounds::sfx_Welcome);    
                    break;

            }

            break;

        default: break;
        
    }  

    #endif

}    
