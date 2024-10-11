// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/Transitions/EndRagdollTransition.h"


#include "DefaultMovementSet/CharacterMoverComponent.h" 
#include "BetterUtilities/public/BetterUtilitiesBPLibrary.h"
#include "RPG_Systems/Mover/Transitions/RagdollTransition.h"

FTransitionEvalResult UEndRagdollTransition::OnEvaluate(const FSimulationTickParams& Params) const
{
	FTransitionEvalResult EvalResult = FTransitionEvalResult::NoTransition;
	if (FRagdollAbilityInputs* AbilityInputs = Params.StartState.InputCmd.InputCollection.FindMutableDataByType<FRagdollAbilityInputs>())
	{
		if (AbilityInputs->bWantsToRagdoll)
		{
			AbilityInputs->bWantsToRagdoll = false;
			if (Params.StartState.SyncState.MovementMode == "Ragdoll")
			{
				EvalResult.NextMode = "Walking";
			}
		}
	}

	return EvalResult;
}


