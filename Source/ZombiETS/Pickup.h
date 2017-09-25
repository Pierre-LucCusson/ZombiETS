// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ZOMBIETS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Return if pickup is active or not
	UFUNCTION(BlueprintPure, category = "Pickup")
	bool IsActive();

	// Allows other classes to safely change if pickup active or not
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	// Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();

	// This function should be overwrite in the children classed for specific actions when pickup is being looted
	virtual void WasCollected_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// True when pickup can be used
	bool bIsActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Return pickup mesh
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

private:
	// Static mesh to represent pickup in the lvl
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta =  (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
};
