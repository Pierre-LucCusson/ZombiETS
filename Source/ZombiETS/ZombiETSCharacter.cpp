// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZombiETS.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "ZombiETSCharacter.h"
#include "Pickup.h"
#include "HealthPickup.h"

//////////////////////////////////////////////////////////////////////////
// AZombiETSCharacter

AZombiETSCharacter::AZombiETSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(100.f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Set a base health amount for the main character
	InitialHealth = 1000.f;
	CharacterHealth = InitialHealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AZombiETSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &AZombiETSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZombiETSCharacter::MoveRight);

	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &AZombiETSCharacter::CollectPickups);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AZombiETSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AZombiETSCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AZombiETSCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AZombiETSCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AZombiETSCharacter::OnResetVR);
}


void AZombiETSCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AZombiETSCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AZombiETSCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AZombiETSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZombiETSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AZombiETSCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AZombiETSCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AZombiETSCharacter::CollectPickups()
{
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// Keep track of the collected health pickup
	float CollectedHealth = 0;

	// For each Actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		// Cast the actor to APickup
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);

		// If the cast is successful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			// Call the pickup's WasCollected function
			TestPickup->WasCollected();

			// Check if the pickup is a HealthPickup
			AHealthPickup* const TestHealthPickup = Cast<AHealthPickup>(TestPickup);
			if (TestHealthPickup) 
			{
				// Increase Health Points
				CollectedHealth += TestHealthPickup->getHealthPoints();
			}

			// Deactivate the pickup
			TestPickup->SetActive(false);
		}
	}
	if (CollectedHealth > 0) 
	{
		UpdateHealth(CollectedHealth);
	}
}

float AZombiETSCharacter::GetInitialHealth()
{
	return InitialHealth;
}

float AZombiETSCharacter::GetCurrentHealth()
{
	return CharacterHealth;
}

void AZombiETSCharacter::UpdateHealth(float HealthChange)
{
	// Change Character'health
	CharacterHealth = FMath::Min(CharacterHealth + HealthChange, InitialHealth);

	// call visual effect
	HealthChangeEffect();
}
