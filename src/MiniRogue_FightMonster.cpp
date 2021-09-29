#include "MiniRogue.h"
#include "Pokitto.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PS = Pokitto::Sound;

const uint8_t monster_captions_offsets[] = { 1, 2, 1, 1, 0, 1, 1, 1, 1, 1 };


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::fightMonster_Init() {

	this->fightMonsterScreenVars.init();
	this->counter = 0;
	this->setDiceSelection(false);

	switch (this->gameState) {

		case GameState::Monster_Init:
			this->fightMonsterScreenVars.monsterStats.hp = (gameStats.level + 1) + random(1, 7); 
			this->fightMonsterScreenVars.monsterStats.dmg = gameStats.getMonsterDMG();
			this->gameState = GameState::Monster;
			break;

		case GameState::BossMonster_Init:
			this->fightMonsterScreenVars.monsterStats.hp = 10 + (gameStats.getAreaId() * 5);
			this->fightMonsterScreenVars.monsterStats.dmg = gameStats.getBossMonsterDMG();
			this->gameState = GameState::BossMonster;
			break;

		case GameState::MonsterFromEvent_Init:
			this->fightMonsterScreenVars.monsterStats.hp = (gameStats.level + 1) * 2;
			this->fightMonsterScreenVars.monsterStats.dmg = 5;
			this->gameState = GameState::MonsterFromEvent;
			break;

		default: break;

	}

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::fightMonster() {

	switch (this->fightMonsterScreenVars.viewState) {

		case FightMonster_ViewState::HighlightMonsterStats:
		case FightMonster_ViewState::HighlightPlayerStats:

			if (PC::buttons.pressed(BTN_A)) { this->counter = FLASH_COUNTER - 1; }
			
			if (this->counter < FLASH_COUNTER) {

				this->counter++;

				if (counter == FLASH_COUNTER) {

					this->counter = 0;
					this->fightMonsterScreenVars.viewState = this->fightMonsterScreenVars.nextState;
					PC::frameCount = 0;

					#ifdef USE_LEDS
					arduboy.setRGBled(RED_LED, 0);
					#endif

				}

			}
			break;

		case FightMonster_ViewState::RollDice:

			this->fightMonsterScreenVars.lastState = FightMonster_ViewState::RollDice;

			if (PC::buttons.pressed(BTN_A)) { 

				this->counter = sizeof(DiceDelay); 

				for (uint8_t i = 0; i < playerStats.xpTrack; i++) {
					if (! this->fightMonsterScreenVars.dice_Retain[i]) this->fightMonsterScreenVars.dice[i] = random(1, 7);
				}

			}

			if (counter < sizeof(DiceDelay)) {
				
				if (PC::frameCount % DiceDelay[counter] == 0) {

					for (uint8_t i = 0; i < playerStats.xpTrack; i++) {
						if (! this->fightMonsterScreenVars.dice_Retain[i]) this->fightMonsterScreenVars.dice[i] = random(1, 7);
					}

					this->counter++;
					PC::frameCount = 0;

				}

			}
			else {


				// Highlight first available dice option ..

				 this->fightMonsterScreenVars.selectedElement = FightMonster_SelectedElement::ItemPoison;
				 this->fightMonsterScreenVars.selectedElement = nextDiceSelection( this->fightMonsterScreenVars.selectedElement);

				setDiceSelection(true); 
				this->fightMonsterScreenVars.viewState = FightMonster_ViewState::DiceSelection;

			}
			break;

		case FightMonster_ViewState::DiceSelection:

			if (PC::buttons.pressed(BTN_LEFT)) 		{  this->fightMonsterScreenVars.selectedElement = prevDiceSelection( this->fightMonsterScreenVars.selectedElement); }
			if (PC::buttons.pressed(BTN_RIGHT))		{  this->fightMonsterScreenVars.selectedElement = nextDiceSelection( this->fightMonsterScreenVars.selectedElement); }
			
			if (PC::buttons.pressed(BTN_A)) {

				switch ( this->fightMonsterScreenVars.selectedElement) {

					case FightMonster_SelectedElement::Dice1 ... FightMonster_SelectedElement::Dice4:

						this->counter = 0;
						this->fightMonsterScreenVars.dice_Retain[static_cast<uint8_t>( this->fightMonsterScreenVars.selectedElement) - 4] = false;
						this->fightMonsterScreenVars.dice_Sixes[static_cast<uint8_t>( this->fightMonsterScreenVars.selectedElement) - 4]++;
						this->fightMonsterScreenVars.viewState = FightMonster_ViewState::RollDice;
						break;

					case FightMonster_SelectedElement::Action:
						{
							uint8_t dmg = getMonsterDMG();

							this->fightMonsterScreenVars.monsterStats.hp = clamp(static_cast<int16_t>(this->fightMonsterScreenVars.monsterStats.hp - dmg), static_cast<uint8_t>(0), static_cast<uint8_t>(30));	
							this->setDiceSelection(false);

							if ( this->fightMonsterScreenVars.monsterStats.hp == 0) { 

								this->monsterIsDead();

							}
							else {
								
								counter = 0;
								this->fightMonsterScreenVars.lastState = FightMonster_ViewState::DiceSelection;
								 this->fightMonsterScreenVars.selectedElement = FightMonster_SelectedElement::Action;

								if (dmg != 0) {

									this->fightMonsterScreenVars.viewState = FightMonster_ViewState::HighlightMonsterStats;

									if (playerStats.itemCount() == 0) {

										if (this->fightMonsterScreenVars.ice == 0) {
											this->fightMonsterScreenVars.nextState = FightMonster_ViewState::Defend;
										}
										else {
											this->fightMonsterScreenVars.nextState = FightMonster_ViewState::RollDice;
										}
										
									}
									else {

										this->fightMonsterScreenVars.nextState = FightMonster_ViewState::WandSelection;

									}

								}
								else {

									if (playerStats.itemCount() == 0) {

										if (this->fightMonsterScreenVars.ice == 0) {
											this->fightMonsterScreenVars.viewState = FightMonster_ViewState::Defend;
										}
										else {
											this->fightMonsterScreenVars.viewState = FightMonster_ViewState::RollDice;
										}

									}
									else {

										this->fightMonsterScreenVars.viewState = FightMonster_ViewState::WandSelection;

									}

								}
								
							}

						}
						break;

					default: break;

				}
			
			}

			break;

		case FightMonster_ViewState::WandSelection:

			this->fightMonsterScreenVars.lastState = FightMonster_ViewState::WandSelection;
	
			if (PC::buttons.pressed(BTN_UP) || PC::buttons.pressed(BTN_LEFT)) 			{  this->fightMonsterScreenVars.selectedElement = this->prevWandSelection(this->fightMonsterScreenVars.selectedElement); }
			if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.pressed(BTN_RIGHT)) 		{  this->fightMonsterScreenVars.selectedElement = this->nextWandSelection(this->fightMonsterScreenVars.selectedElement); }
			
			if (PC::buttons.pressed(BTN_A)) {

				switch (this->fightMonsterScreenVars.selectedElement) {

					case FightMonster_SelectedElement::ItemFire:

						this->playerStats.items[static_cast<uint8_t>(Wand::Fire)]--;
						this->fightMonsterScreenVars.monsterStats.hp = clamp(static_cast<int16_t>(this->fightMonsterScreenVars.monsterStats.hp - 10), static_cast<uint8_t>(0), static_cast<uint8_t>(30));	
						this->setDiceSelection(false);

						if ( this->fightMonsterScreenVars.monsterStats.hp == 0) { 

							this->monsterIsDead();

						}
						else {

							this->fightMonsterScreenVars.lastState = FightMonster_ViewState::WandSelection;
							this->fightMonsterScreenVars.viewState = FightMonster_ViewState::ItemFireUsed;

						}
						break;

					case FightMonster_SelectedElement::ItemIce:
						this->fightMonsterScreenVars.ice = 1;
						this->fightMonsterScreenVars.viewState = FightMonster_ViewState::ItemIceUsed;
						playerStats.items[static_cast<uint8_t>(Wand::Ice)]--;
						break;

					case FightMonster_SelectedElement::ItemPoison:
						this->fightMonsterScreenVars.poison = true;
						this->fightMonsterScreenVars.viewState = FightMonster_ViewState::ItemPoisonUsed;
						playerStats.items[static_cast<uint8_t>(Wand::Poison)]--;
						break;

					case FightMonster_SelectedElement::ItemHealing:
						this->fightMonsterScreenVars.viewState = FightMonster_ViewState::ItemHealingUsed;
						playerStats.items[static_cast<uint8_t>(Wand::Healing)]--;
						playerStats.incHP(8);
						break;

					default: 
						if (this->fightMonsterScreenVars.ice == 0) {
							this->fightMonsterScreenVars.viewState = FightMonster_ViewState::Defend;
						}
						else {
							this->fightMonsterScreenVars.viewState = FightMonster_ViewState::RollDice;
						}
						break;

				}
			
			}

			break;

		case FightMonster_ViewState::Defend:

			playerStats.decHP(clamp(static_cast<int16_t>(this->fightMonsterScreenVars.monsterStats.dmg - playerStats.armour), static_cast<uint8_t>(0), static_cast<uint8_t>(50)));

			if (playerStats.hp == 0) {

				this->fightMonsterScreenVars.viewState = FightMonster_ViewState::HighlightPlayerStats;
				this->fightMonsterScreenVars.nextState = FightMonster_ViewState::PlayerDead;

			}
			else {

				this->setDiceSelection(false);
				this->fightMonsterScreenVars.viewState = FightMonster_ViewState::HighlightPlayerStats;
				this->fightMonsterScreenVars.nextState = FightMonster_ViewState::RollDice;
			}
			
			break;

		case FightMonster_ViewState::MonsterDead:

			if (PC::buttons.pressed(BTN_A)) { 
				this->gameStats.monsterDefeated = true;
        		this->gameState = gameStats.incRoom(playerStats); 
			}
			
			if (this->counter < FLASH_COUNTER) {

				this->counter++;

				if (counter == FLASH_COUNTER) {

					this->fightMonsterScreenVars.lastState = this->fightMonsterScreenVars.viewState;
					this->fightMonsterScreenVars.viewState = FightMonster_ViewState::MonsterDead_Wait;

				}

			}

			break;

		case FightMonster_ViewState::MonsterDead_Wait:

			if (PC::buttons.pressed(BTN_A)) {

				this->gameStats.monsterDefeated = true;
        		this->gameState = gameStats.incRoom(playerStats);

				#ifdef USE_LEDS
				arduboy.setRGBled(RED_LED, 0);
				#endif

			}

			break;

		case FightMonster_ViewState::PlayerDead:

			if (PC::buttons.pressed(BTN_A)) {
				this->gameState = GameState::GameOver_Init;

				#ifdef USE_LEDS
				arduboy.setRGBled(RED_LED, 0);
				#endif

			}

			break;

		case FightMonster_ViewState::ItemIceUsed:

			if (PC::buttons.pressed(BTN_A)) {
				this->fightMonsterScreenVars.viewState = FightMonster_ViewState::RollDice;
			}

			break;
			
		case FightMonster_ViewState::ItemFireUsed:
		case FightMonster_ViewState::ItemPoisonUsed:
		case FightMonster_ViewState::ItemHealingUsed:

			if (PC::buttons.pressed(BTN_A)) {
				this->fightMonsterScreenVars.viewState = FightMonster_ViewState::Defend;
			}

			break;

	}

	this->renderFightMonster();

}


void Game::monsterIsDead() {

	switch (this->gameState) {

		case GameState::Monster:
			this->playerStats.incXP(gameStats.getMonsterReward());
			break;

		case GameState::BossMonster:
		 	{
				this->playerStats.incXP(gameStats.getAreaId() + 2);
				this->playerStats.incGold(gameStats.getAreaId() < 2 ? 2 : 3);
				this->playerStats.bossesKilled++;

				this->fightMonsterScreenVars.diceMonster = random(1, 7);
				if (this->playerStats.itemCount() >= 2 && this->fightMonsterScreenVars.diceMonster < 5) this->fightMonsterScreenVars.diceMonster = 7;

				switch (this->fightMonsterScreenVars.diceMonster) {

					case 1 ... 4:   playerStats.items[this->fightMonsterScreenVars.diceMonster - 1]++; break;
					case 5:         playerStats.incArmour(1); break;
					case 6:         playerStats.incXP(2); break;
					case 7:         playerStats.incGold(2); break;

					break;

				}

			}

		  break;

		case GameState::MonsterFromEvent:
			this->playerStats.incXP(2);
			break;

		default: break;

	}

	this->fightMonsterScreenVars.viewState = FightMonster_ViewState::MonsterDead;
	
}

void Game::drawMonsterHead(uint8_t const *imageHead) {

	uint8_t head_inc = (this->fightMonsterScreenVars.monsterPosition == 1 || this->fightMonsterScreenVars.monsterPosition == 2 ? 1 : 0);

	PD::drawBitmap(63, 0 + head_inc, imageHead);

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void Game::renderFightMonster() {

	bool flash = PC::frameCount % FLASH_DELAY < (FLASH_DELAY / 2);

	uint8_t hand_inc = (this->fightMonsterScreenVars.monsterPosition == 2 || this->fightMonsterScreenVars.monsterPosition == 3 ? 1 : 0);


	// Update monster animation ..

	this->fightMonsterScreenVars.monsterPositionInc++; 
	if (this->fightMonsterScreenVars.monsterPositionInc > 3) {

		this->fightMonsterScreenVars.monsterPositionInc = 0;

		this->fightMonsterScreenVars.monsterPosition++; 
		if (this->fightMonsterScreenVars.monsterPosition > 4) this->fightMonsterScreenVars.monsterPosition = 0;

	}


	// Draw background ..

  	this->renderBackground();
	PD::drawBitmap(14, 1, Images::Monster_Stats);
	PD::drawBitmap(54, 19, Images::Monster_LowerBody);
	PD::drawBitmap(44, 1 + hand_inc, Images::Monster_Sword, NOROT, FLIPH);

	{

		switch (this->gameState) {

			case GameState::BossMonster:

				this->drawMonsterHead(Images::BossMonster_Head);
				PD::drawBitmap(105, 1 + hand_inc, Images::Monster_Sword);				
				
				break;

			default:

				this->drawMonsterHead(Images::Monster_Head);
				PD::drawBitmap(84, 18 + hand_inc, Images::Monster_Shield);

				break;

		}

	}


	// Monster statistics ..
	{

		if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::HighlightMonsterStats) {

			if (flash) {
				PD::setColor(7);
				PD::fillRect(31, 1, ( this->fightMonsterScreenVars.monsterStats.hp < 10 ? 5 : 10), 6);
				PD::setColor(0, 7);
				#ifdef USE_LEDS
				//arduboy.setRGBled(0, 32, 0);
				#endif
			}
			else {

				#ifdef USE_LEDS
				//arduboy.setRGBled(0, 0, 0);
				#endif
			}

		}

		PD::setCursor(32, 2);
		PD::print(static_cast<uint16_t>(this->fightMonsterScreenVars.monsterStats.hp));
		PD::setColor(7, 0);
		PD::setCursor(32, 10);
		PD::print(static_cast<uint16_t>(this->fightMonsterScreenVars.monsterStats.dmg));

	}


	// Inventory ..

	if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::WandSelection || this->fightMonsterScreenVars.lastState == FightMonster_ViewState::WandSelection) {

		PD::drawBitmap(14, 22, Images::Monster_Items);

		for (uint8_t i = 0; i < 4; i++) {
			PD::setCursor(32, 24 + (i*10));
			PD::print(static_cast<uint16_t>(playerStats.items[i]));
		}
	
	}


	// Dice ..

	if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::DiceSelection || this->fightMonsterScreenVars.viewState == FightMonster_ViewState::RollDice || this->fightMonsterScreenVars.lastState == FightMonster_ViewState::DiceSelection) {

		uint8_t captionIndex = gameStats.getAreaId() + (this->gameState == GameState::BossMonster ? 5 : 0);
		PD::setCursor(14, 22 + (monster_captions_offsets[captionIndex] * 4));

		switch (captionIndex) {

			case 0:
				PD::setCursor(14, 22);
				PD::print("Undead");
				PD::setCursor(14, 30);
				PD::print("Soldier");
				break;

			case 1:
				PD::setCursor(14, 22);
				PD::print("Skeleton");
				break;

			case 2:
				PD::setCursor(14, 22);
				PD::print("Undead");
				PD::setCursor(14, 30);
				PD::print("Knight");
				break;

			case 3:
				PD::setCursor(14, 22);
				PD::print("Serpent");
				PD::setCursor(14, 30);
				PD::print("Knight");
				break;

			case 4:
				PD::setCursor(14, 22);
				PD::print("Ogs");
				PD::setCursor(14, 30);
				PD::print("Sanctum");
				PD::setCursor(14, 38);
				PD::print("Guard");
				break;

			case 5:
				PD::setCursor(14, 22);
				PD::print("Undead");
				PD::setCursor(14, 30);
				PD::print("Giant");
				break;

			case 6:
				PD::setCursor(14, 22);
				PD::print("Skeleton");
				PD::setCursor(14, 30);
				PD::print("Lord");
				break;

			case 7:
				PD::setCursor(14, 22);
				PD::print("Undead");
				PD::setCursor(14, 30);
				PD::print("Lord");
				break;

			case 8:
				PD::setCursor(14, 22);
				PD::print("Serpent");
				PD::setCursor(14, 30);
				PD::print("Demon");
				break;

			case 9:
				PD::setCursor(14, 22);
				PD::print("Ogs");
				PD::setCursor(14, 30);
				PD::print("Remains");
				break;
				
		}

		for (uint8_t i = 0; i < playerStats.xpTrack; i++) {
			PD::drawBitmap(14 + (i * 10), 52, Images::Dice[this->fightMonsterScreenVars.dice[i]]);
		}
	}


	// Marker ..
	
	if (( this->fightMonsterScreenVars.selectedElement == FightMonster_SelectedElement::Action && flash) || ( this->fightMonsterScreenVars.selectedElement != FightMonster_SelectedElement::Action)) {

		uint8_t x = 0;
		uint8_t y = 0;

		if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::DiceSelection) 			{ x = 14 + (playerStats.xpTrack * 10);	y = 53;	}
		if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::WandSelection) 			{ x = 37; y = 53;	}

		if (x > 0) PD::drawBitmap(x, y, Images::Marker, 0);

	}


	// Highlighted item ..
	{

		if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::DiceSelection || this->fightMonsterScreenVars.viewState == FightMonster_ViewState::WandSelection) {

		  if (!flash) {
			
				uint8_t x = 0;
				uint8_t y = 0;

				switch ( this->fightMonsterScreenVars.selectedElement) {

					case FightMonster_SelectedElement::ItemFire ... FightMonster_SelectedElement::ItemHealing:
						x = 14;
						y = 22 + (static_cast<uint8_t>( this->fightMonsterScreenVars.selectedElement) * 10);
						break;

					case FightMonster_SelectedElement::Dice1 ... FightMonster_SelectedElement::Dice4:
						x = 14 + ((static_cast<uint8_t>( this->fightMonsterScreenVars.selectedElement) - 4) * 10);
						y = 52;
						break;

					default: break;
		
				}

				if (x > 0) PD::drawBitmap(x, y, Images::Monster_Item_Highlight, 0);

			}

		}

	}


	// Player statistics ..

	const FlashSettings settings = ((this->fightMonsterScreenVars.viewState == FightMonster_ViewState::HighlightPlayerStats) ? FlashSettings::FlashHP : FlashSettings::None);

	#ifdef USE_LEDS
	if (this->fightMonsterScreenVars.viewState == FightMonster_ViewState::HighlightPlayerStats) {
		arduboy.setRGBled(RED_LED, (flash ? 32 : 0));
	}
	#endif
	this->renderPlayerStatistics(true, settings);


	// Messages ..

	switch (this->fightMonsterScreenVars.viewState) {

		case FightMonster_ViewState::HighlightPlayerStats:
	
			if (this->fightMonsterScreenVars.nextState == FightMonster_ViewState::PlayerDead) {
				this->renderPlayerDead();
			}

			break;
			
		case FightMonster_ViewState::MonsterDead:
		case FightMonster_ViewState::MonsterDead_Wait:

			if (this->fightMonsterScreenVars.viewState != FightMonster_ViewState::MonsterDead_Wait) {

				FlashSettings settings = FlashSettings::FlashXP;

				if(this->gameState == GameState::BossMonster) {

					settings = static_cast<FlashSettings>(static_cast<uint8_t>(settings) | static_cast<uint8_t>(FlashSettings::FlashGold));
					if(this->fightMonsterScreenVars.diceMonster == 5) settings = static_cast<FlashSettings>(static_cast<uint8_t>(settings) | static_cast<uint8_t>(FlashSettings::FlashArmour));
					
				}

				this->renderPlayerStatistics(true, settings);

			}

			if (this->gameState == GameState::BossMonster) {

    	 		this->renderMessageBox(11, 23, 88, 26);
			 	PD::setCursor(17, 29);
			 	PD::print("You killed the Boss.\n");
			 	PD::setCursor(17, 38);

				switch (this->fightMonsterScreenVars.diceMonster - 1) {

					case 0:
						PD::print(" Gain a Fire Wand!");
						break;

					case 1:
						PD::print(" Gain an Ice Wand!");
						break;

					case 2:
						PD::print("Gain a Poison Wand!");
						break;

					case 3:
						PD::print("Gain a Healing Wand!");
						break;

					case 4:
						PD::print("  Gain an Armour!");
						break;

					case 5:
						PD::print("Upgrade your Weapon!");
						break;

					case 6:
						PD::print("Gain 2 Gold Pieces!");
						break;
						
				}

			}
			else {
			
				this->renderMonsterDead();

			}
			break;

		case FightMonster_ViewState::PlayerDead:

			this->renderPlayerDead();
			break;

		case FightMonster_ViewState::ItemIceUsed:
		case FightMonster_ViewState::ItemFireUsed:
		case FightMonster_ViewState::ItemPoisonUsed:
		case FightMonster_ViewState::ItemHealingUsed:

   	 		this->renderMessageBox(3, 17, 106, 32);
			PD::setCursor(31, 21);

			switch ( static_cast<uint8_t>(this->fightMonsterScreenVars.viewState) - static_cast<uint8_t>(FightMonster_ViewState::ItemIceUsed)) {

				case 0:
					PD::setCursor(8, 22);
					PD::print("  You used an ice spell.");
					PD::setCursor(8, 30);
					PD::print("    Monster is frozen");
					PD::setCursor(8, 38);
					PD::print("        for one turn.");
					break;

				case 1:
					PD::setCursor(8, 22);
					PD::print(" You used a fire spell.");
					PD::setCursor(8, 30);
					PD::print("    Monster loses 10");
					PD::setCursor(8, 38);
					PD::print("      HP points.");
					break;

				case 2:
					PD::setCursor(8, 22);
					PD::print(" You used a poison spell.");
					PD::setCursor(8, 30);
					PD::print("    Deal 2 extra damage");
					PD::setCursor(8, 39);
					PD::print("           per turn.");
					break;

				case 3:
					PD::setCursor(8, 22);
					PD::print("You used a healing spell.");
					PD::setCursor(8, 30);
					PD::print("     You have gained 8");
					PD::setCursor(8, 38);
					PD::print("      extra HP points.");
					break;

			}

			break;

		default: break;

	}

}



// ----------------------------------------------------------------------------
//  Get the previous dice selection.  Dices need to be 6 to select ..
//
FightMonster_SelectedElement Game::prevDiceSelection(FightMonster_SelectedElement element) {

	if (element > FightMonster_SelectedElement::Dice4 && this->fightMonsterScreenVars.dice[3] == 6) 			{ return FightMonster_SelectedElement::Dice4; }
	if (element > FightMonster_SelectedElement::Dice3 && this->fightMonsterScreenVars.dice[2] == 6) 			{ return FightMonster_SelectedElement::Dice3; }
	if (element > FightMonster_SelectedElement::Dice2 && this->fightMonsterScreenVars.dice[1] == 6) 			{ return FightMonster_SelectedElement::Dice2; }
	if (element > FightMonster_SelectedElement::Dice1 && this->fightMonsterScreenVars.dice[0] == 6) 			{ return FightMonster_SelectedElement::Dice1; }

	return element;

}


// ----------------------------------------------------------------------------
//  Get the next dice or action selection.  Dices need to be 6 to select ..
//
FightMonster_SelectedElement Game::nextDiceSelection(FightMonster_SelectedElement element) {

	if (element < FightMonster_SelectedElement::Dice1 && this->fightMonsterScreenVars.dice[0] == 6) 			{ return FightMonster_SelectedElement::Dice1; }
	if (element < FightMonster_SelectedElement::Dice2 && this->fightMonsterScreenVars.dice[1] == 6) 			{ return FightMonster_SelectedElement::Dice2; }
	if (element < FightMonster_SelectedElement::Dice3 && this->fightMonsterScreenVars.dice[2] == 6) 			{ return FightMonster_SelectedElement::Dice3; }
	if (element < FightMonster_SelectedElement::Dice4 && this->fightMonsterScreenVars.dice[3] == 6) 			{ return FightMonster_SelectedElement::Dice4; }
	if (element < FightMonster_SelectedElement::Action) 											{ return FightMonster_SelectedElement::Action; }

	return element;

}


// ----------------------------------------------------------------------------
//  Get the previous wand selection ..
//
FightMonster_SelectedElement Game::prevWandSelection(FightMonster_SelectedElement element) {

	if (element > FightMonster_SelectedElement::ItemHealing && this->playerStats.items[static_cast<uint8_t>(Wand::Healing)] > 0) 		{ return FightMonster_SelectedElement::ItemHealing; }
	if (element > FightMonster_SelectedElement::ItemFire && this->playerStats.items[static_cast<uint8_t>(Wand::Fire)] > 0) 				{ return FightMonster_SelectedElement::ItemFire; }
	if (element > FightMonster_SelectedElement::ItemIce && this->playerStats.items[static_cast<uint8_t>(Wand::Ice)] > 0) 				{ return FightMonster_SelectedElement::ItemIce; }
	if (element > FightMonster_SelectedElement::ItemPoison && this->playerStats.items[static_cast<uint8_t>(Wand::Poison)] > 0) 			{ return FightMonster_SelectedElement::ItemPoison; }

	return element;

}


// ----------------------------------------------------------------------------
//  Get the next wand or action selection ..
//
FightMonster_SelectedElement Game::nextWandSelection(FightMonster_SelectedElement element) {
	
	if (element < FightMonster_SelectedElement::ItemHealing && this->playerStats.items[static_cast<uint8_t>(Wand::Healing)] > 0) 		{ return FightMonster_SelectedElement::ItemHealing; }
	if (element < FightMonster_SelectedElement::ItemFire && this->playerStats.items[static_cast<uint8_t>(Wand::Fire)] > 0) 				{ return FightMonster_SelectedElement::ItemFire; }
	if (element < FightMonster_SelectedElement::ItemIce && this->playerStats.items[static_cast<uint8_t>(Wand::Ice)] > 0) 				{ return FightMonster_SelectedElement::ItemIce; }
	if (element < FightMonster_SelectedElement::ItemPoison && this->playerStats.items[static_cast<uint8_t>(Wand::Poison)] > 0) 			{ return FightMonster_SelectedElement::ItemPoison; }
	if (element < FightMonster_SelectedElement::Action) 																				{ return FightMonster_SelectedElement::Action; }

	return element;

}


// ----------------------------------------------------------------------------
//  Get the damage inflicted on the monster this roll of the dice ..
//
uint8_t Game::getMonsterDMG() {

	uint8_t damage = 0;

	for (uint8_t i = 0; i < this->playerStats.xpTrack; i++) {
		if (this->fightMonsterScreenVars.dice[i] != 1) {
			damage = damage + this->fightMonsterScreenVars.dice[i] + (this->fightMonsterScreenVars.dice_Sixes[i] * 6);
		}
	}

	return damage + (fightMonsterScreenVars.poison ? 5 : 0);

}


// ----------------------------------------------------------------------------
//  Clear dice selection array ready to roll again ..
//
void Game::setDiceSelection(bool value) {

	for (uint8_t i = 0; i < this->playerStats.xpTrack; i++) {

		 this->fightMonsterScreenVars.dice_Retain[i] = value;

		if (!value) {
			this->fightMonsterScreenVars.dice_Sixes[i] = 0;
			if (this->fightMonsterScreenVars.ice > 0) this->fightMonsterScreenVars.ice--;
		}

	}

}
