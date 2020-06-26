/* Main character. The character is controlled by player input (keyboard + mouse), has a skeletal mesh and a
	camera set to follow at a distance for a third person view. The character can interact with pickups and hazards,
	and has combat functionality, being	able to equip and use a weapon. */

#include "Main.h"
#include "Enemy.h"
#include "FirstSaveGame.h"
#include "MainPlayerController.h"
#include "Weapon.h"
#include "WeaponContainerActor.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

	// Sets default values
AMain::AMain()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Hardcoding capsule size (can still be changed in blueprints)
	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);

	// Create camera boom (pulls towards player if there's a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // camera follows at this distance
	CameraBoom->bUsePawnControlRotation = true;  //rotate arm based on controller 

	// Create follow camera and attack it to the spring arm
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // as only cameraboom should follow the controller's rotation 

	// Turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// Disable character rotation if only the controller rotates
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Orient character's rotation to camera movement based on rotation rate
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.0f);

	// Jump velocity and control
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f; // freedom of movement while in air

	// Player stats - defaults
	MaxHealth = 100.f;
	Health = 65.f;
	MaxStamina = 150.f;
	Stamina = 120.f;
	Coins = 0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bSprintKeyDown = false;
	bLMBDown = false;
	bESCDown = false;

	// Initializing enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	// Initializing all other variables
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	bAttacking = false;

	bHasCombatTarget = false;

	bMovingForward = false;
	bMovingRight = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	// Get the controller for the main character and start a game
	MainPlayerController = Cast<AMainPlayerController>(GetController());
	LoadGameNoSwitch();

	// Set the input to just the gamemode
	if (MainPlayerController)
	{
		MainPlayerController->GameModeOnly();
	}
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Do not go ahead with any functionality if the character is dead
	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	// Stamina drain over time
	float DeltaStamina = DeltaTime * StaminaDrainRate;

	// Switching between states (enums) 
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		// If the stamina status is normal, drain the stamina (if the shift key is pressed) or replenish it (if the shift key is not pressed), and change stamina status if required
		if (bSprintKeyDown)
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}

			if (bMovingForward || bMovingRight)
				// Update movement status
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}

		}
		else // sprint key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_BelowMinimum:
		// If the stamina is below a minimum value, drain the stamina till it reaches 0 (if the shift key is pressed) or replenshi it (if the shift key is not pressed), and update movement status
		if (bSprintKeyDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				if (bMovingForward || bMovingRight)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
		}
		else // sprint key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_Exhausted:
		// If the stamina is exhausted, the character cannot sprint. If the shift key is not pressed, replenish stamina 
		if (bSprintKeyDown)
		{
			Stamina = 0.f;
		}
		else //sprint key up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:
		// If the stamina is in this state, the character cannot sprint till the stamina reaches a minimum value
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

	default:
		;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		// Functionality to turn (change actor's rotation) to face the enemy while attacking
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		// If the character has a combat target, get the enemy location
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{
	// Get the direction that the character is facing (yaw rotation)
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Check to make sure player input is valid
	check(PlayerInputComponent);

	// Bind functions to keyboard input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::SprintKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::SprintKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	// Control camera using mouse
	PlayerInputComponent->BindAxis("Turn", this, &AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMain::LookUp);

	// Control camera using keyboard keys
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
}

bool AMain::CanMove(float Value)
{
	// Check if the player can move (if the character is not dead/attacking/in the pause menu and has an input)
	return (Controller != nullptr) &&
		(Value != 0.0f) &&
		(!bAttacking) &&
		(MovementStatus != EMovementStatus::EMS_Dead) &&
		!MainPlayerController->bPauseMenuOpen;
}

void AMain::Turn(float Value)
{
	// Turn by adding yaw input
	if (CanMove(Value))
	{
		AddControllerYawInput(Value);
	}
}

void AMain::LookUp(float Value)
{
	// Look up by adding pitch input
	if (CanMove(Value))
	{
		AddControllerPitchInput(Value);
	}
}

void AMain::MoveForward(float Value)
{
	// Functionality for the character to move in the direction that it's facing

	bMovingForward = false;
	if (CanMove(Value))
	{
		// Find which way is 'forward' based on controller rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // pitch and roll are zeroed out as only the yaw should be considered

		// Get rotation matrix from yaw rotation, and perform a dot operation to get the movement direction
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Add input in the calculated direction, based on the input value
		AddMovementInput(Direction, Value);

		// Toggle moving boolean
		bMovingForward = true;
	}
}

void AMain::MoveRight(float Value)
{
	// (Similar functionality as with the MoveForward function above)

	bMovingRight = false;
	if (CanMove(Value))
	{
		// Find which way is 'forward' based on controller rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); // pitch and roll are zeroed out as only the yaw should be considered

		// Get rotation matrix from yaw rotation, and perform a dot operation to get the movement direction
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add input in the calculated direction, based on the input value
		AddMovementInput(Direction, Value);

		// Toggle moving boolean
		bMovingForward = true;
	}
}

void AMain::TurnAtRate(float Rate)
{
	/* Take input from key presses and add to the controller, depending on the turn rate, and add rotation each frame (based on
		DeltaTime from the Tick function */
	float a = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	float b = FMath::Clamp(a, -1.0f, 1.0f);
	AddControllerYawInput(Rate);
}

void AMain::LookUpAtRate(float Rate)
{
	/* Take input from key presses and add to the controller, depending on the lookup rate, and add rotation each frame (based on
	DeltaTime from the Tick function */
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LMBDown()
{
	// If LMB is pressed, unless the character is dead, perform functionality

	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	// Check to see if there is an overlap, and then if LMB is pressed, equip item
	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}

	// If an equipped item already exists, perform an attack instead
	else if (EquippedWeapon)
	{
		Attack();
	}
}

void AMain::LMBUp()
{
	// LMB released
	bLMBDown = false;
}

void AMain::ESCDown()
{
	// ESC pressed
	bESCDown = true;

	// Get pause menu
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

void AMain::ESCUp()
{
	// ESC key released
	bESCDown = false;
}

void AMain::DecrementHealth(float Amount)
{
	// Decrement health by damage amount, and call the die function if health is 0 or below
	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

void AMain::IncrementHealth(float Amount)
{
	// Increment health by boost amount unless it is already at its max value
	if (Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::Die()
{
	// Upon main character's death, play death animation and update movement status

	if (MovementStatus == EMovementStatus::EMS_Dead) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}

	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::Jump()
{
	// Perform a jump unless the character is dead 
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

void AMain::DeathEnd()
{
	// To stop moving once the character has died
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	// Set the movement status and set the character's speed to either sprinting or running, depending on the status

	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMain::SprintKeyDown()
{
	// Shift key pressed
	bSprintKeyDown = true;
}

void AMain::SprintKeyUp()
{
	// Shift key released
	bSprintKeyDown = false;
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	// Equip a weapon, or if already equipped, replace current weapon with a new one
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;
}

void AMain::Attack()
{
	if (!bAttacking && (MovementStatus != EMovementStatus::EMS_Dead))
	{
		// Initialize attack variable and turn on functionality to face enemy while attacking
		bAttacking = true;
		SetInterpToEnemy(true);

		// Play anim montage
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			// To pick between two attack types randomly

			int32 Selection = FMath::RandRange(0, 1);
			switch (Selection)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;

			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;

			default:
				;
			}
		}
	}
}

void AMain::AttackEnd()
{
	// Turn off attack functionality unless the LMB is pressed again
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

void AMain::PlaySwingSound()
{
	// Play sound cue when weapon is swung
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AMain::SetInterpToEnemy(bool Interp)
{
	// For the character to automatically face the enemy when close enough
	bInterpToEnemy = Interp;
}

float AMain::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Decrement health or die based on damage amount, and return that damage amount
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();

		if (DamageCauser)
		{
			// If the main character has been defeated by an enemy, clear enemy's target
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}

	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AMain::UpdateCombatTarget()
{
	// Functionality to set the main character's combat target to the enemy that is closest to the character

	// Create an array of enemies from overlapping actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	// If there are no enemies, do not display enemy health bar
	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	// Initialize closest enemy
	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		// Get distance from enemy to actor
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		// Loop through all enemies and find the closest enemy based on least distance to the actor
		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();

				// If a closer enemy is found, update the ClosestEnemy variable
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}

		// Display the closest enemy's health bar and update combat target properties 
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}

void AMain::SwitchLevel(FName LevelName)
{
	// If the current map name is not the level name, switch level
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
			FString Level = LevelName.ToString();
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

void AMain::SaveGame()
{
	// Save Game functionality - saves stats, map, and weapon if there is one, to a slot
	UFirstSaveGame* SaveObject = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveObject->CharacterStats.Health = Health;
	SaveObject->CharacterStats.MaxHealth = MaxHealth;
	SaveObject->CharacterStats.Stamina = Stamina;
	SaveObject->CharacterStats.MaxStamina = MaxStamina;
	SaveObject->CharacterStats.Coins = Coins;
	SaveObject->CharacterStats.Location = GetActorLocation();
	SaveObject->CharacterStats.Rotation = GetActorRotation();

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	SaveObject->CharacterStats.LevelName = MapName;
	if (EquippedWeapon)
	{
		SaveObject->CharacterStats.WeaponName = EquippedWeapon->Name;
		SaveObject->CharacterStats.bWeaponParticles = EquippedWeapon->bWeaponParticles;
	}

	UGameplayStatics::SaveGameToSlot(SaveObject, SaveObject->PlayerName, SaveObject->UserIndex);
}

void AMain::LoadGame(bool LoadPosition)
{
	// Load Game functionality - loads stats, switches level if required, and loads weapon
	UFirstSaveGame* Load = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	UFirstSaveGame* LoadObject = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(Load->PlayerName, Load->UserIndex));

	if (LoadObject)
	{
		if (LoadObject->CharacterStats.LevelName != "")
		{
			FName Map(*LoadObject->CharacterStats.LevelName);
			SwitchLevel(Map);
		}

		Health = LoadObject->CharacterStats.Health;
		MaxHealth = LoadObject->CharacterStats.MaxHealth;

		Stamina = LoadObject->CharacterStats.Stamina;
		MaxStamina = LoadObject->CharacterStats.MaxStamina;

		Coins = LoadObject->CharacterStats.Coins;

		// If player had an equipped weapon when SaveGame was called, equip the weapon
		if (WeaponContainer)
		{
			AWeaponContainerActor* Container = GetWorld()->SpawnActor<AWeaponContainerActor>(WeaponContainer);
			if (Container)
			{
				FString WeaponName = LoadObject->CharacterStats.WeaponName;
				if (Container->WeaponMap.Num() > 0)
				{
					if (Container->WeaponMap.Contains(WeaponName))
					{
						AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Container->WeaponMap[WeaponName]);
						if (Weapon)
						{
							Weapon->bWeaponParticles = LoadObject->CharacterStats.bWeaponParticles;
							Weapon->Equip(this);

						}
					}
				}
			}
		}
		if (LoadPosition)
		{
			SetActorLocation(LoadObject->CharacterStats.Location);
			SetActorRotation(LoadObject->CharacterStats.Rotation);
		}
	}
}

void AMain::LoadGameNoSwitch()
{
	// Similar functionality to the LoadGame function above, but without switching the level
	UFirstSaveGame* Load = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	UFirstSaveGame* LoadObject = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(Load->PlayerName, Load->UserIndex));

	if (LoadObject)
	{
		Health = LoadObject->CharacterStats.Health;
		MaxHealth = LoadObject->CharacterStats.MaxHealth;

		Stamina = LoadObject->CharacterStats.Stamina;
		MaxStamina = LoadObject->CharacterStats.MaxStamina;

		Coins = LoadObject->CharacterStats.Coins;
	}
}
