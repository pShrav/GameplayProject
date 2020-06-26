// Volume for the player to walk into, to trigger a transition to another level/map

#include "LevelTransitionVolume.h"
#include "Main.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
	// Set this actor to call Tick() every frame - in this case it doesn't need to tick as the event does not happen every frame
	PrimaryActorTick.bCanEverTick = false;

	// Creating box and billboard components for the transition volume
	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	// Initializing the level name to the first map
	TransitionLevelName = "SunTemple";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();

	// Adding overlap functionality to the transition volume. OverlapEnd is not considered in this case
	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Checking the other actor and casting it to main (as only the main character overlap should be considered)
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			// Switch character to another level/map
			Main->SwitchLevel(TransitionLevelName);
		}
	}
}
