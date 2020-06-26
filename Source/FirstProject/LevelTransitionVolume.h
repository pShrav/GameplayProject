// Volume for the player to walk into, to trigger a transition to another level/map

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

UCLASS()
class FIRSTPROJECT_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelTransitionVolume();

	// Box component for the level transition volume
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Transition)
		class UBoxComponent* TransitionVolume;

	// Billboard to make box visible in the editor
	class UBillboardComponent* Billboard;

	// FName to store the name of the level
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Transition)
		FName TransitionLevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overlap function
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
