// Animation instance for the main character

#include "MainAnimInstance.h"
#include "Main.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	// If the pawn is null, get owner of anim instance and cast it to main
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Main = Cast<AMain>(Pawn);
		}
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	// If the pawn is null, get owner of anim instance
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	// Get pawn's (lateral) movement speed and 'in air' status if the pawn is valid
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		// If the main is null, cast it to the pawn class
		if (Main == nullptr)
		{
			Main = Cast<AMain>(Pawn);
		}
	}
}
