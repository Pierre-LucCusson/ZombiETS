// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIZombieController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIETS_API AAIZombieController : public AAIController
{
	GENERATED_BODY()

	//Behavior tree component
	UBehaviorTreeComponent* BehaviorComp;

	//Blackboard component
	UBlackboardComponent* BlackboardComp;

	//Blackboard keys
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKey;
	
	TArray<AActor*> ZombiePoints;

	virtual void Possess(APawn* Pawn) override;

public:

	AAIZombieController();

	void SetPlayerCaught(APawn* Pawn);

	int32 CurrentZombiePoint = 0;

	//Inline getter functions
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	FORCEINLINE TArray<AActor*> GetZombiePoints() const { return ZombiePoints; }
	
	
};
