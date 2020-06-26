// A volume in which an item or enemy can spawn

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class FIRSTPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Box component for the spawn volume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawning)
		class UBoxComponent* SpawningBox;

	/* Creating a TSubClassOf to create variables that can be set from blueprints, to select an actor to spawn in the volume
		(in this case, four different actors can be selected to spawn) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning)
		TSubclassOf<AActor> Actor_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning)
		TSubclassOf<AActor> Actor_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning)
		TSubclassOf<AActor> Actor_3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning)
		TSubclassOf<AActor> Actor_4;

	// Array to store the above spawn volumes
	TArray<TSubclassOf<AActor>> SpawnArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Blueprint functions with no inputs, to get spawnpoint and actor to spawn
	UFUNCTION(BlueprintPure, Category = Spawning)
		FVector GetSpawnPoint();
	UFUNCTION(BlueprintPure, Category = Spawning)
		TSubclassOf<AActor> GetSpawnActor();

	// Function that can be called from C++ and have hybrid functionality between C++ and UE
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Spawning)
		void SpawnOurActor(UClass* ToSpawn, const FVector& Location);
};
