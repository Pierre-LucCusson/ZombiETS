// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombiETS.h"
#include "BTSelectZombiePoint.h"

//le reste a ete ajouter
#include "AIZombiePoint.h"
#include "AIZombieController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTSelectZombiePoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory)
{
	AAIZombieController* AICon = Cast<AAIZombieController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		//Gey BB component
		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();

		AAIZombiePoint* CurrentPoint = Cast<AAIZombiePoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

		TArray<AActor*> AvailableZombiePoints = AICon->GetZombiePoints();

		AAIZombiePoint* NextZombiePoint = nullptr;

		if (AICon->CurrentZombiePoint != AvailableZombiePoints.Num() - 1)
		{
			NextZombiePoint = Cast<AAIZombiePoint>(AvailableZombiePoints[++AICon->CurrentZombiePoint]);
		}
		else // if there is not any points to go to
		{
			NextZombiePoint = Cast<AAIZombiePoint>(AvailableZombiePoints[0]);
			AICon->CurrentZombiePoint = 0;
		}

		BlackboardComp->SetValueAsObject("LocationToGo", NextZombiePoint);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}



