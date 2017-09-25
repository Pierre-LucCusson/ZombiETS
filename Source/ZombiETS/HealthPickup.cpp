// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "HealthPickup.h"


//Set default values
AHealthPickup::AHealthPickup()
{
	Super::GetMesh()->SetSimulatePhysics(true);

	// health amount regen when looted
	healthPoints = 100.f;
}

void AHealthPickup::WasCollected_Implementation()
{
	// Use the base pickup behavior
	Super::WasCollected_Implementation();
	//Destroy the battery to simulate the pickup
	Destroy();
}

// Returns healthPoints collected
float AHealthPickup::getHealthPoints()
{
	return healthPoints;
}
