/* Enemy character with a skeletal mesh and an AI controller. The enemy has two collision spheres - when the
	main character overlaps with the enemy's outer 'AggroSphere', the enemy moves towards the main character,
	and starts attacking once the main character overlaps with the inner 'CombatSphere'. The enemy takes damage
	when the main character attacks and the weapon collides with the enemy's capsule. */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

	// Creating enemy states using enum
UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle						UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget		UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking				UMETA(DisplayName = "Attacking"),
	EMS_Dead					UMETA(DisplayName = "Dead"),

	EMS_MAX						UMETA(DisplayName = "DefaultMAX") // default for last item
};

UCLASS()
class GAMEPLAYPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Check to see if the enemy has a valid target
	bool bHasValidTarget;

	// Enemy state variable with a getter and setter for it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		EEnemyMovementStatus EnemyMovementStatus;
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	// Adding collision spheres for enemy aggro, and attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		class USphereComponent* AggroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		USphereComponent* CombatSphere;

	// Collision volume for enemy's attack
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat)
		class UBoxComponent* CombatCollision;

	// Giving enemy an attack delay
	FTimerHandle AttackTimer;

	// Declaring AI controller for the enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		class AAIController* AIController;

	// Consequent enemy attacks can be separated by a random time delay between mintime and maxtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AttackMaxTime;

	// Damage type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TSubclassOf<UDamageType> DamageTypeClass;

	// Enemy stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		float Damage;

	// Timer and delay for enemy's death (for enemy to be destroyed)
	FTimerHandle DeathTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float DeathDelay;

	// Particles and sounds for enemy damage and attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class USoundCue* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USoundCue* SwingSound;

	// Using combat montage depending on enemy state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		class UAnimMontage* CombatMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Overlap functions for the enemy's outer aggrosphere
	UFUNCTION()
		virtual void AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Function for AI to move to character
	UFUNCTION(BlueprintCallable)
		void MoveToTarget(class AMain* Target);

	// Overlap functions for the enemy's inner combatsphere
	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Check if player is overlapping with enemy's combat sphere, for the enemy to attack
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bOverlappingCombatSphere;

	// Overlap functions for the enemy socket (that deals damage)
	UFUNCTION()
		void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// To identify the combat target
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		AMain* CombatTarget;

	// Functions to toggle collision, to play a sound only when the enemy's attacking socket overlaps with the main character's capsule
	UFUNCTION(BlueprintCallable)
		void ActivateCollision();
	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	// Check to see if the enemy is attacking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		bool bAttacking;

	// Enemy's attack function to deal damage
	void Attack();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	// Check if the enemy is not dead
	bool Alive();

	// Function to decrement enemy's health and implement death
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Die(AActor* Causer);

	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	// Destroy the enemy after it dies
	void Disappear();
};
