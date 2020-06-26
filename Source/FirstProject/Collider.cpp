// This is a test asset (pawn), to test functionality of features like the camera and movement

#include "Collider.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACollider::ACollider()
{
	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Initial Setup: creating sphere and mesh components and attaching them to a created root component 
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(40.f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	// Setting a sphere-shaped mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	// Checking if the mesh has been set, and assigning initial location and rotation values
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	// Creating a springarm, attaching it to the root at an angle, distance, and lag speed, for the camera to lag behind the pawn for smooth viewing
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// Creating a camera component and attaching it to the spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Creating a movement component and setting it to the root (adding movement input to the root of the pawn)
	MyMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("MyMovementComponent"));
	MyMovementComponent->UpdatedComponent = RootComponent;

	// Initializing camera input vector
	CameraInput = FVector2D(0.f, 0.f);

	// To possess the pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Changing pawn rotation (yaw) by adding camera input (X) every frame
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	/* Changing spring arm rotation (instead of rotating pawn upwards/downwards) in a similar way
		The Y-input is clamped to prevent pawn from "seeing upside-down" */
	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);
	SpringArm->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Adding movement functionality to the pawn and camera by binding instead of changing location
	   (Binding needs to be set to user inputs in UE4 project settings) */
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
}

// Function called when key corresponding to 'moving forward' is pressed
void ACollider::MoveForward(float Input)
{
	// The input is between -1 and 1 and is scaled by the pawn's forward movement, if the component is valid
	FVector Forward = GetActorForwardVector();

	if (MyMovementComponent)
	{
		MyMovementComponent->AddInputVector(Input * Forward);
	}
}

// Function called when key corresponding to 'moving right' is pressed
void ACollider::MoveRight(float Input)
{
	// The input is between -1 and 1 and is scaled by the pawn's right movement, if the component is valid
	FVector Right = GetActorRightVector();

	if (MyMovementComponent)
	{
		MyMovementComponent->AddInputVector(Input * Right);
	}
}

// Function called when key corresponding to 'camera pitch' is pressed
void ACollider::PitchCamera(float AxisValue)
{
	// Directly adding input to camera Y input for pitch rotation
	CameraInput.Y = AxisValue;
}

// Function called when key corresponding to 'camera yaw' is pressed
void ACollider::YawCamera(float AxisValue)
{
	// Directly adding input to camera X input for yaw rotation
	CameraInput.X = AxisValue;
}

// Getting the movement component
UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return MyMovementComponent;
}
