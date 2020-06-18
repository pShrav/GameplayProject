// Platform that can 'float' between two points with a delay.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AFloatingPlatform();

	// Static mesh for the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Platform)
		class UStaticMeshComponent* Mesh;

	// Start point for the platform to float from 
	UPROPERTY(EditAnywhere, Category = Platform)
		FVector StartPoint;

	// End point for the platform to float to (the meta enables visualization of the vector) 
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"), Category = Platform)
		FVector EndPoint;

	// Floating speed, time, and distance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Platform)
		float InterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Platform)
		float InterpTime;
	float Distance;

	// Creating a delay timer for the platform to wait before returning
	FTimerHandle InterpTimer;

	// Check if interping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Platform)
		bool bInterping;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// To edit the value of bInterping
	void ToggleInterping();

	// Swap start and end vectors for platform to float back to starting point
	void SwapVectors(FVector& VecOne, FVector& VecTwo);
};
