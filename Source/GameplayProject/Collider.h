// This is a test asset (pawn), to test functionality of features like the camera and movement

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class GAMEPLAYPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Creating a static mesh, visible in the UE Editor
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* MeshComponent;

	// Creating a sphere component to bind this asset to
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USphereComponent* SphereComponent;

	// Camera component for the asset
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UCameraComponent* Camera;

	// SpringArm component to set camera at a distance and enable smooth viewing
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USpringArmComponent* SpringArm;

	// Movement component
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UColliderMovementComponent* MyMovementComponent;

	// Getter for the movement component, overriding the existing function in the Pawn class
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// Inine getter and setter for the mesh component
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }

	// Inline getter and setter for the sphere component
	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }
	FORCEINLINE void SetSphereComponent(USphereComponent* Sphere) { SphereComponent = Sphere; }

	// Getter and setter for the camera component
	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera) { Camera = InCamera; }

	// Getter and setter for the springarm component
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArm; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArm) { SpringArm = InSpringArm; }

private:

	// Pawn movement functions
	void MoveForward(float Input);
	void MoveRight(float Input);

	// Camera movement functions
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	// Camera input as a 2D Vector
	FVector2D CameraInput;
};
