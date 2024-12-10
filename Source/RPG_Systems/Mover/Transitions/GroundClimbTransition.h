// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementModeTransition.h"
#include "RPG_Systems/Mover/RPG_BaseMoverCharacter.h"
#include "GroundClimbTransition.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UGroundClimbTransition : public UBaseMovementModeTransition
{
	GENERATED_BODY()
public:
	virtual FTransitionEvalResult OnEvaluate(const FSimulationTickParams& Params) const override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool CanClimb = false;
};
