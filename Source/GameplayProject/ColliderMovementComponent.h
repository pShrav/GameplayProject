// Movement component for the Collider pawn that includes functionality to slide along surface on collision.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ColliderMovementComponent.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API UColliderMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
