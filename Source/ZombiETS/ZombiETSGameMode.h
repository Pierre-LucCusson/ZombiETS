// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ZombiETSGameMode.generated.h"

UCLASS(minimalapi)
class AZombiETSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZombiETSGameMode();

	virtual void Tick(float DeltaTime) override;

protected:
	// health decreasing rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float HealthDecayRate;
};





