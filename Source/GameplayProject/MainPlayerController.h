/* Controller for the main character (HUD funtionality - pause menu, enemy health bar, and HUD overlay for player
	health and stamina). */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

	// Reference to UMG asset in editor (Unreal Motion Graphics) to create HUD overlay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> HUDOverlayAsset;

	// Variable to hold HUD widget after creating it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* HUDOverlay;

	// Enemy health bar widget and variable to hold it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<UUserWidget> WEnemyHealthBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* EnemyHealthBar;

	// Pause menu widget and variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<UUserWidget> WPauseMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* PauseMenu;

	// To check if the pause menu should be visible
	bool bPauseMenuVisible;

	// Functions to display, remove, or toggle the pause menu based on above boolean
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HUD)
		void DisplayPauseMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HUD)
		void RemovePauseMenu();

	void TogglePauseMenu();

	// Check to see if the pause menu is open
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		bool bPauseMenuOpen;

	// Check and functions for enemy health bar visibility 
	bool bEnemyHealthBarVisible;
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;

	// Function to set input mode
	void GameModeOnly();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
