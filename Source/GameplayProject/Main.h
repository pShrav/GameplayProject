/* Main character. The character is controlled by player input (keyboard + mouse), has a skeletal mesh and a
	camera set to follow at a distance for a third person view. The character can interact with pickups and hazards,
	and has combat functionality, being	able to equip and use a weapon. */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

	// Enum for movement states (to switch between animations)
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	EMS_Dead		UMETA(DisplayName = "Dead"),

	EMS_MAX			UMETA(DisplayName = "DefaultMAX") // default for last item
};

// Enum for stamina states (for stamina bar functionality)
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal							UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum				UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted						UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering	UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX								UMETA(DisplayName = "DefaultMAX")  // default for last item
};

// Struct to store player stats
USTRUCT(BlueprintType)
struct  FCharacterStats
{
	GENERATED_BODY()

	// Player stats
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		float Health;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		float MaxHealth;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		float Stamina;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		int32 Coins;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		FVector Location;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		FRotator Rotation;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		FString WeaponName;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = SaveGameData)
		bool bWeaponParticles;
	UPROPERTY(VisibleAnywhere, Category = SaveGameData)
		FString LevelName;
};

UCLASS()
class GAMEPLAYPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	/*
	/* Player stats
	*/
	FCharacterStats CharacterStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		int32 Coins; // 32 bit integer, this is a cross-platform version of int

		// Functions to increment coins/health, and decrement health
	UFUNCTION(BlueprintCallable)
		void IncrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
		void IncrementHealth(float Amount);
	void DecrementHealth(float Amount);

	// Function to take damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();


	/*
	/* Camera
	*/

	// Camera boom positioning camera behind player (spring arm component)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// Follow camera (camera component)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// Functionality to let character turn without using the mouse (turn rate to scale turning function for camera)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/*
	/* Stamina functionality (draining/replenishing based on character's movement)
	*/

	// For sprinting by pressing down the shift key
	bool bSprintKeyDown;
	void SprintKeyDown(); // press to enable sprinting
	void SprintKeyUp();		 // release to stop sprinting

	// State variables for movement and stamina 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enums)
		EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
		EStaminaStatus StaminaStatus;

	// Setters and getters for enums, to set status and running speed
	void SetMovementStatus(EMovementStatus Status);
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	// Character's speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Running)
		float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Running)
		float SprintingSpeed;

	// Variables for stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float StaminaDrainRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MinSprintStamina;


	/*
	/* Combat functionality
	*/

	// Current combat target to be attacked
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		class AEnemy* CombatTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat)
		FVector CombatTargetLocation;

	// Interping attack to face enemy
	FRotator GetLookAtRotationYaw(FVector Target);
	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	// Check to see if character has a combat target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		bool bHasCombatTarget;

	// Setters for combat target 
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	// Particles and sounds to play on hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		class UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class USoundCue* HitSound;

	// Override jump function if dead
	virtual void Jump() override;

	// Variable to access controller (for enemy health bar)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller)
		class AMainPlayerController* MainPlayerController;

	// To store equipped weapon when savegame is called
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeaponContainerActor> WeaponContainer;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Character movement functions/properties
	void MoveForward(float Value);
	void MoveRight(float Value);
	bool bMovingForward;
	bool bMovingRight;
	bool CanMove(float Value);

	// Yaw rotation and pitch rotation
	void Turn(float Value);
	void LookUp(float Value);

	// Called via input to turn/look at a given (normalized) rate 
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Getters for camera boom and follow camera
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	// LMB functionality (for equipping weapon or attacking)
	bool bLMBDown;
	void LMBDown();
	void LMBUp();

	// To check the item that the player overlapped with (pickup/explosive)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
		class AItem* ActiveOverlappingItem;

	// Weapon equipped by character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
		class AWeapon* EquippedWeapon;

	// Setter to set equipped weapon and active overlapping item
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* ItemToSet) { ActiveOverlappingItem = ItemToSet; }

	// Combat functionality
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anims)
		bool bAttacking;
	void Attack();
	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	// Update combat target once the current combat target has been defeated
	void UpdateCombatTarget();

	// To play anim montage for combat 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anims)
		class UAnimMontage* CombatMontage;

	// Sound to play on weapon swing
	UFUNCTION(BlueprintCallable)
		void PlaySwingSound();

	// To fill an array with only enemy class or its derivatives
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TSubclassOf<AEnemy> EnemyFilter;

	// Functionality following character's death
	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	// Esc functionality (for pause menu)
	bool bESCDown;
	void ESCDown();
	void ESCUp();

	// To store map name while switching levels
	void SwitchLevel(FName LevelName);

	// Save and load functions
	UFUNCTION(BlueprintCallable)
		void SaveGame();

	UFUNCTION(BlueprintCallable)
		void LoadGame(bool LoadPosition);

	// To load a game without switching the level
	UFUNCTION(BlueprintCallable)
		void LoadGameNoSwitch();
};
