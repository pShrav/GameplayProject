/* Controller for the main character (HUD funtionality - pause menu, enemy health bar, and HUD overlay for player
	health and stamina). */

#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{
	// Initialize boolean
	bPauseMenuOpen = false;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Check if overlay is valid then create HUD widget
	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	// Add asset to viewport and make it visible
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	// Enemy health bar widget: create widget, add to viewport and make visible, and set horizontal alignment
	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);

		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	// Pause menu widget: create and add to viewport
	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Display the enemy health bar (toggle boolean and set visibility)
void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

// Remove the enemy health bar (toggle boolean and change visibility)
void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		// Functionality to place enemy health bar directly above the enemy (each frame)
		FVector2D PositionInViewport;

		// Get enemy's location as a 2D vector on the screen 
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 85.f;

		// Size of the enemy health bar
		FVector2D SizeInViewport = FVector2D(300.f, 25.f);

		// Set enemy health bar to the desired size and height above the enemy
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	// Display the pause menu (toggle boolean and set visibility)
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		// Change input to mouse cursor to access pause menu
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		// Hide the enemy health bar (toggle boolean and set visibility)
		GameModeOnly();
		bPauseMenuVisible = false;
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	// Simple toggle function based on the visibility boolean
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::GameModeOnly()
{
	// Change input mode to game input
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
