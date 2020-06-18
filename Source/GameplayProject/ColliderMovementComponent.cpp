// Movement component for the Collider pawn, that includes functionality to slide along surface on collision.

#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Calling the parent version as it is a virtual function
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Checking to make sure everything is still valid, and that the pawn has an owner and can move
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// Getting and clearing 'addinput' vector from Collider and clamping it to a max speed value, and setting it to movement vector for this frame
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.f;

	// If movement this frame is almost zero, call SafeMove function
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If pawn bumps into something, slide along the side of the object
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time /* remaining time after collision*/, Hit.Normal, Hit);
		}
	}
}
