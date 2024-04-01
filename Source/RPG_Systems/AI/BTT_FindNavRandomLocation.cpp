// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/AI/BTT_FindNavRandomLocation.h"
#include "NavigationSystem.h"
#include <BehaviorTree/BTFunctionLibrary.h>
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_FindNavRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!OwnerPawn)return EBTNodeResult::Failed;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)return EBTNodeResult::Failed;
	 
	if (OwnerPawn) {
		const FVector Location = OwnerPawn->GetActorLocation();
		FVector RandomLoc = FVector::ZeroVector;
		bool Sucess = UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this,Location, RandomLoc,Radius);
		BlackboardComp->SetValueAsVector(RandomLocation.SelectedKeyName,RandomLoc);
		 
		if (Sucess) {
			return EBTNodeResult::Succeeded;
		}
		else {
			return EBTNodeResult::Failed;
		}
	
	}
	return EBTNodeResult::Failed;
}
