// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementModeTransition.h"
#include "EndRagdollTransition.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UEndRagdollTransition : public UBaseMovementModeTransition
{
	GENERATED_BODY()
	
	
	
	
	FTransitionEvalResult Evaluate_Implementation(const FSimulationTickParams& Params) const override;

};
