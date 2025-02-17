// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "WaveManager.h"

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
	~AZombiETSGameMode();

	virtual void Tick(float DeltaTime) override;

	// Returns health to lose, used by HUD
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthToLose();

	// Returns wave number
	UFUNCTION(BlueprintPure, Category = "Wave")
	int GetWaveNumber();

	// Returns wave music name
	UFUNCTION(BlueprintPure, Category = "Wave")
	FString GetWaveMusicName();

	// Returns wave number
	UFUNCTION(BlueprintPure, Category = "Wave")
	int GetZombieToKillBeforeNextWave();

	virtual void BeginPlay() override;

	// currentState Getter
	UFUNCTION(BlueprintPure, Category = "Health")
	EHealthPlayState GetCurrentState() const;

	// currentState Setter
	UFUNCTION()
	void SetCurrentState(EHealthPlayState NewState);

	// Set the game to pause
	void Pause();

	//Set lighting throught the current level
	void SetLighting(float lighting);

	//Set all player speed
	void SetPlayerSpeed(float speed);

	//Set all zombie speed
	void SetZombieSpeed(float speed);

	//Manage the wave
	void ManageWave(ZombiETSWave* wave);

	//Start next wave
	void StartNextWave();

	int ZombieKilled(int zombies = 0);

	void SetLoading(bool load = true);

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

	// The current wave number
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave", Meta = (BlueprintProtected = "true"))
	int waveNumber;

	// The current wave time in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave", Meta = (BlueprintProtected = "true"))
	int waveTime;

	// The name of the wave music
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave", Meta = (BlueprintProtected = "true"))
	FString waveMusic;

	// Wave Widget class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> WaveHUDWidgetClass;

	// Zombies To kill in the current wave
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave", Meta = (BlueprintProtected = "true"))
	int ZombiesToKill;

	// Wave Widget class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> DeathWidgetClass;

	// Loading Widget class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Loading", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

	// Instance of health widget class
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	// Playing status
	UPROPERTY()
	EHealthPlayState CurrentState;

	// Wave Manager
	WaveManager* waveManager;

	// Zombie killed in the current wave
	int ZombieKilledInWave;

	// Instance of health widget class
	UPROPERTY()
	class UUserWidget* LoadingWidget;

	void Dead();
};





