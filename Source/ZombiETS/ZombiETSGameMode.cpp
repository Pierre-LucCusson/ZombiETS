// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZombiETS.h"
#include "ZombiETSGameMode.h"
#include "ZombiETSCharacter.h"
#include "Kismet/GameplayStatics.h"

AZombiETSGameMode::AZombiETSGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// base health decay rate
	HealthDecayRate = 0.01f;
}

void AZombiETSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AZombiETSCharacter* MyCharacter = Cast<AZombiETSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter) 
	{
		// If still alive
		if (MyCharacter->GetCurrentHealth() > 0) 
		{
			// Decrease Health overTime
			MyCharacter->UpdateHealth(-DeltaTime * HealthDecayRate * (MyCharacter->GetInitialHealth()));
		}
	}
}