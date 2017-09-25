// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTSelectZombiePoint.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIETS_API UBTSelectZombiePoint : public UBTTaskNode
{
	GENERATED_BODY()

	//le reste a ete ajouter
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
