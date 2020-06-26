// A switch that can be placed on the floor, and when stepped on by the main character, can trigger a door to open

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloorSwitch : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloorSwitch();

	// Creating trigger box (overlap volume for functionality to be triggered)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FloorSwitch)
		class UBoxComponent* TriggerBox;

	// Mesh component for the floor switch 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FloorSwitch)
		class UStaticMeshComponent* FloorSwitch;

	// Mesh component for the door to move when the switch is stepped on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FloorSwitch)
		UStaticMeshComponent* Door;

	// Initial switch location before overlap
	UPROPERTY(BlueprintReadWrite, Category = FloorSwitch)
		FVector InitialSwitchLocation;

	// Initial door location before overlap
	UPROPERTY(BlueprintReadWrite, Category = FloorSwitch)
		FVector InitialDoorLocation;

	// Timerhandle struct to add a delay for door to remain open
	FTimerHandle SwitchHandle;

	// Delay time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloorSwitch)
		float SwitchTime;

	// Check to see if character is on switch
	bool bCharacterOnSwitch;

	// Once the timer is up, close the door
	void CloseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overlap begin and overlap end functions for the floor switch
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Functions to raise and lower the door and switch (functionality provided in blueprints for smoother transitioning)
	UFUNCTION(BlueprintImplementableEvent, Category = FloorSwitch)
		void RaiseDoor();
	UFUNCTION(BlueprintImplementableEvent, Category = FloorSwitch)
		void LowerDoor();
	UFUNCTION(BlueprintImplementableEvent, Category = FloorSwitch)
		void RaiseFloorSwitch();
	UFUNCTION(BlueprintImplementableEvent, Category = FloorSwitch)
		void LowerFloorSwitch();

	// Functions written in C++ for updating door and floor switch locations (only Z is considered as the door and switch move in just one direction)
	UFUNCTION(BlueprintCallable, Category = FloorSwitch)
		void UpdateDoorLocation(float Z);
	UFUNCTION(BlueprintCallable, Category = FloorSwitch)
		void UpdateFloorSwitchLocation(float Z);
};
