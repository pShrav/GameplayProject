// Animation instance for the main character

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

UCLASS()
class FIRSTPROJECT_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	// Constructor to initialize animation
	virtual void NativeInitializeAnimation() override;

	// Tick function to update animation properties each frame
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties();

	// Character's movement speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

	// Check to see if the character is in the air 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsInAir;

	// Reference to pawn and main
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AMain* Main;
};
