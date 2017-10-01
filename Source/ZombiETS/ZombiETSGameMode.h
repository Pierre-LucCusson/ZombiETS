// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ZombiETSGameMode.generated.h"

// current state gameplay
UENUM(BlueprintType)
enum class EHealthPlayState : uint8
{
	EPlaying,
	EGameOver
};

UCLASS(minimalapi)
class AZombiETSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZombiETSGameMode();

	virtual void Tick(float DeltaTime) override;

	// Returns health to lose, used by HUD
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthToLose();

	virtual void BeginPlay() override;

	// currentState Getter
	UFUNCTION(BlueprintPure, Category = "Health")
	EHealthPlayState GetCurrentState() const;

	// currentState Setter
	UFUNCTION()
	void SetCurrentState(EHealthPlayState NewState);

protected:
	// health decreasing rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	float HealthDecayRate;

	// The health 0 die condition
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	float HealthToLose;

	// HUG Widget class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// Instance of widget class
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	// Playing status
	UPROPERTY()
	EHealthPlayState CurrentState;
};





