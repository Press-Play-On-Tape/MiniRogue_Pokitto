#pragma once

#include "Pokitto.h"
#include "PokittoCookie.h"
#include "Constants.h"


class GameCookie : public Pokitto::Cookie {

	public:

		uint8_t initialised;
		uint8_t highScore;
		SoundEffects sfx = SoundEffects::Both;

	public:

		void initialise() {

			this->initialised = COOKIE_INITIALISED;
			this->sfx = SoundEffects::Both;
			this->highScore = 0;
			this->saveCookie();

		}

};