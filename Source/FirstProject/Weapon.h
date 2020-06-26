/* Weapon that can be picked up and equipped by the player. When the weapon is equipped, and the attack function
	is called, the weapon deals damage to an enemy if the weapon's collision box overlaps with the enemy's capsule. */

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

	// Creating an enum for weapon states, to deal damage only when equipped
UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	EWS_Pickup			UMETA(DisplayName = "Pickup"),
	EWS_Equipped		UMETA(DisplayName = "Equipped"),

	EWS_MAX				UMETA(DisplayName = "DefaultMAX")   // default for last item
};

UCLASS()
class FIRSTPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	// Skeletal mesh for the weapon (as Item has a static mesh)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
		class USkeletalMeshComponent* SkeletalMesh;

	// Box collision for combat, for the weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		class UBoxComponent* CombatCollision;

	// Damage to be dealt by the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		float Damage;

	// Weapon state variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
		EWeaponState WeaponState;

	// Boolean to select if particles should be attached to the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		bool bWeaponParticles;

	// Equip and swing sound cues 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		class USoundCue* OnEquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		USoundCue* SwingSound;

	// Weapon name to be saved as a string (while saving the game)
	UPROPERTY(EditDefaultsOnly, Category = SavedData)
		FString Name;


protected:
	virtual void BeginPlay() override;


public:

	// Inheriting overlap functions from main
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Function for the main character to equip weapon
	void Equip(class AMain* Char);

	// Getters and setters for weapon state
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	// Overlap functions for when the weapon is used in combat (overlap between the weapon and the enemy)
	UFUNCTION()
		void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// To decide when collision occurs in terms of animation
	UFUNCTION(BlueprintCallable)
		void ActivateCollision();
	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	// To deal damage to enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TSubclassOf<UDamageType> DamageTypeClass;

	// Get controller from main when applying damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		AController* WeaponInstigator;
	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
};
