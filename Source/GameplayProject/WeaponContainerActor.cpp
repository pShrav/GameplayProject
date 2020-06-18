// To save a weapon by its name when SaveGame is called

#include "WeaponContainerActor.h"

// Sets default values
AWeaponContainerActor::AWeaponContainerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponContainerActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponContainerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
