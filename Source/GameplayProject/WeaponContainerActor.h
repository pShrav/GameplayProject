// To save a weapon by its name when SaveGame is called

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponContainerActor.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API AWeaponContainerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponContainerActor();

	// Creating a TMap to store items with their defaults ([key] -> [value])
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TMap<FString, TSubclassOf<class AWeapon>> WeaponMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
