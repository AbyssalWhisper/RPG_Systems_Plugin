// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTT_FindNavRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UBTT_FindNavRandomLocation : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FBlackboardKeySelector RandomLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius = 500;
};
