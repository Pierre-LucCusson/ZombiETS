// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZombiETS.h"
#include "ZombiETSGameMode.h"
#include "ZombiETSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"


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

	waveManager = new WaveManager();
}

int AZombiETSGameMode::GetWaveNumber()
{
	return waveNumber;
}

FString AZombiETSGameMode::GetWaveMusicName()
{
	return waveMusic;
}

void AZombiETSGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EHealthPlayState::EPlaying);

	// Set the health to lose
	AZombiETSCharacter* MyCharacter = Cast<AZombiETSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter)
	{
		HealthToLose = 0.0f;
	}
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
	if (WaveHUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WaveHUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	waveManager->StartNextWave();
}

AZombiETSGameMode::~AZombiETSGameMode()
{
	delete waveManager;
}

void AZombiETSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AZombiETSCharacter* MyCharacter = Cast<AZombiETSCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter) 
	{
		// if game over (DEAD)
		if (MyCharacter->GetCurrentHealth() <= HealthToLose)
		{
			Dead();
		}
		// If still alive
		else if (MyCharacter->GetCurrentHealth() > 0) 
		{
			// Decrease Health overTime
			MyCharacter->UpdateHealth(-DeltaTime * HealthDecayRate * (MyCharacter->GetInitialHealth()));
		}
	}

	waveNumber = waveManager->CurrentWave()->GetNumber();
	waveMusic = waveManager->CurrentWave()->GetName();
}

float AZombiETSGameMode::GetHealthToLose()
{
	return 0.0f;
}

EHealthPlayState AZombiETSGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AZombiETSGameMode::SetCurrentState(EHealthPlayState NewState)
{
	CurrentState = NewState;
}

void AZombiETSGameMode::Pause() {
	APlayerController* player = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (player != NULL) {
		player->SetPause(true);
	}
}

void AZombiETSGameMode::Dead()
{
	SetCurrentState(EHealthPlayState::EGameOver);
	waveManager->StopWave();
	Pause();
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}
	APlayerController* PC = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}

}
