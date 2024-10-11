// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementModeTransition.h"
#include "BP_BaseMovementModeTransition.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UBP_BaseMovementModeTransition : public UBaseMovementModeTransition
{
	GENERATED_BODY()

protected:
	virtual FTransitionEvalResult OnEvaluate(const FSimulationTickParams& Params) const override;
	virtual void OnTrigger(const FSimulationTickParams& Params) override;
};
