// Explosive item that deals damage to the main character or enemy upon overlap.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API AExplosive : public AItem
{
	GENERATED_BODY()

public:

	// Constructor
	AExplosive();

	// Damage that will be dealt to player on contact with explosive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float Damage;

	// Overlap begin and overlap end functions
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Creating a damage type class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TSubclassOf<UDamageType> DamageTypeClass;
};
