// Functionality that updates enemy animation based on its movement speed.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Constructor to initialize animation
	virtual void NativeInitializeAnimation() override;

	// Creating an update function similar to the Tick function
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties();

	// Enemy movement speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

	// Reference to pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	// Reference to enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AEnemy* Enemy;
};
