// Functionality that updates enemy animation based on its movement speed.

#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	// Checking to see if the pawn is null, and if so, getting pawn owner
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		// Checking to see if the pawn is valid and casting it to the Enemy class
		if (Pawn)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	// Similar check and cast as above
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}

	// Checking to see if the pawn is valid
	if (Pawn)
	{
		// Obtaining the pawn's velocity
		FVector Speed = Pawn->GetVelocity();

		// Removing the Z-movement component (only lateral movement is considered)
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);

		// Getting the magnitude of the vector as a float to set it as the movement speed
		MovementSpeed = LateralSpeed.Size();
	}
}
