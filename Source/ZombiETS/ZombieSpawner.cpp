// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "ZombieSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIZombie.h"


// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the box component to represent spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	// Set the spawn delay range
	SpawnDelayRangeLow = 2.0f;
	SpawnDelayRangeHigh = 5.0f;
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AZombieSpawner::SpawnZombie, SpawnDelay, false);
}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AZombieSpawner::GetRandomPointInVolume()
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void AZombieSpawner::SpawnZombie()
{
	if (WhatToSpawn != NULL)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			// params
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// Get random location to spawn at
			FVector SpawnLocation = GetRandomPointInVolume();

			// Get a random rotation for the spawned zombie
			FRotator SpawnRotation;
			SpawnRotation.Yaw = 0;
			SpawnRotation.Pitch = 0;
			SpawnRotation.Roll = 0;

			// Spawn the zombie
			AAIZombie* const SpawnedZombie = World->SpawnActor<AAIZombie>(WhatToSpawn, SpawnLocation, SpawnRotation);

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AZombieSpawner::SpawnZombie, SpawnDelay, false);
		}
	}
}


