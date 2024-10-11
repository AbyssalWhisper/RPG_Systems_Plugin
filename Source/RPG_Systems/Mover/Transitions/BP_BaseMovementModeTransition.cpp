// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/Transitions/BP_BaseMovementModeTransition.h"


FTransitionEvalResult UBP_BaseMovementModeTransition::OnEvaluate(const FSimulationTickParams& Params) const
{
	return K2_OnEvaluate(Params);
}

void UBP_BaseMovementModeTransition::OnTrigger(const FSimulationTickParams& Params)
{
	K2_OnTrigger(Params);
}
