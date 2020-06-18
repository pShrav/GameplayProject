// Data to be saved when SaveGame is called

#include "FirstSaveGame.h"

UFirstSaveGame::UFirstSaveGame()
{
	// Setting inittial values for player stats
	PlayerName = TEXT("Default");
	UserIndex = 0;
	CharacterStats.WeaponName = TEXT("");
	CharacterStats.LevelName = TEXT("");
}
