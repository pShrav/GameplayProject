// Platform that can float between two points with a delay.

#include "FloatingPlatform.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create a static mesh and attach it to the root of the component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Initialize values
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	bInterping = false;

	InterpSpeed = 4.f;
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Setting start point to platform's initial location
	StartPoint = GetActorLocation();

	// Adding platform location to get endpoint
	EndPoint += StartPoint;

	// Setting a timer to get platform back after a delay
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	// Get size of the vector to obtain distance
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		// Platform's current location each frame
		FVector CurrentLocation = GetActorLocation();

		// Interpolating to position in space
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		// Setting actor's location to the interp location
		SetActorLocation(Interp);

		// Get distance travelled this frame
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

		// If the full distance has been covered, stop interping and turn back (swap vectors)
		if (Distance - DistanceTraveled <= 1.f)
		{
			ToggleInterping();

			// Add delay before platform returns
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}

}

void AFloatingPlatform::ToggleInterping()
{
	// Toggle interping
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	// Swapping vectors
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}
