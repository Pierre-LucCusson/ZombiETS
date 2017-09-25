// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIETS_API AHealthPickup : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AHealthPickup();

	// Override the WasCollected function - use Implementation because it's a Blueprint Native Event
	void WasCollected_Implementation() override;

	// Health regen amount getter
	float getHealthPoints();

protected:
	// Set the amount of health regen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	float healthPoints;
};
