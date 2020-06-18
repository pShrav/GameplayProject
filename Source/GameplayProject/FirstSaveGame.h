// Data to be saved when SaveGame is called

#pragma once

#include "Main.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFirstSaveGame();

	// Player name
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString PlayerName;

	// Save slot
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	// Player stats to be saved
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FCharacterStats CharacterStats;
};
