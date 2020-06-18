// A volume in which an item or enemy can spawn

#include "SpawnVolume.h"
#include "AIController.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Box component for the spawn volume
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	// If all actors are valid, add them to the array
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	// Get a random point in spawn volume (between the origin and extent of the box) and return it
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}

void ASpawnVolume::SpawnActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		// Spawn parameter for the actor, to use in the SpawnActor function
		FActorSpawnParameters SpawnParams;

		// To spawn an actor, get the World variable and check if it is valid
		UWorld* World = GetWorld();

		if (World)
		{
			// Construct and spawns (return) the actor			
			AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams);

			// Check to see if the actor is a valid enemy, and then spawn AI controller for the pawn 
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				Enemy->SpawnDefaultController();

				// Get enemy controller to enable the enemy to move around the spawn volume
				AAIController* AICont = Cast<AAIController>(Enemy->GetController());
				if (AICont)
				{
					Enemy->AIController = AICont;
				}
			}
		}
	}
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	// If there are valid actors in the spawn array, randomly select one (out of the four) to spawn
	if (SpawnArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, (SpawnArray.Num() - 1));
		return SpawnArray[Selection];
	}
	else
	{
		return nullptr;
	}
}
