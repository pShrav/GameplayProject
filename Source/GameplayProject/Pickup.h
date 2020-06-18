// Item that can be picked up by the player, such as a coin

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Pickup.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API APickup : public AItem
{
	GENERATED_BODY()

public:

	APickup();

	// Overlap functions
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Function to be implemented in blueprints on pickup event
	UFUNCTION(BlueprintImplementableEvent, Category = Pickup)
		void OnPickupBP(class AMain* Target);
};
