// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "AIZombie.h"
#include "AIZombieController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAIZombie::AAIZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//Init senses
	PawnSensingComp = CreateEditorOnlyDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

}

// Called when the game starts or when spawned
void AAIZombie::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIZombie::OnPlayerCaught);
	}
	
}

// Called every frame
//void AAIZombie::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

// Called to bind functionality to input
void AAIZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
}

void AAIZombie::OnPlayerCaught(APawn * Pawn)
{
	//Get a reference to the player controller
	AAIZombieController* AIController = Cast<AAIZombieController>(GetController());

	if (AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Zombie: I can see yuou !!!"));
		AIController->SetPlayerCaught(Pawn);
	}
}

