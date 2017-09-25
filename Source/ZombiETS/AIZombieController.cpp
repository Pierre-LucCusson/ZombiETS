// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "AIZombieController.h"
#include "AIZombie.h"
#include "AIZombiePoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAIZombieController::AAIZombieController()
{
	//Init blackboard and behavior tree
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	//Init blackboard keys
	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";

	CurrentZombiePoint = 0;
}

void AAIZombieController::SetPlayerCaught(APawn* Pawn)
{
	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerKey, Pawn);
	}
}

void AAIZombieController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	//Get reference to the character
	AAIZombie* AICharacter = Cast<AAIZombie>(Pawn);

	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset) 
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
		}

		//Populate zombie point array
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIZombiePoint::StaticClass(), ZombiePoints);

		BehaviorComp->StartTree(*AICharacter->BehaviorTree);
	}
}
