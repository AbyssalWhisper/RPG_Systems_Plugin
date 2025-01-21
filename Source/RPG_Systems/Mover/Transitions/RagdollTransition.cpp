// Fill out your copyright notice in the Description page of Project Settings.
#include "RPG_Systems/Mover/Transitions/RagdollTransition.h"

#include "DefaultMovementSet/CharacterMoverComponent.h" 
#include "BetterUtilities/public/BetterUtilitiesBPLibrary.h"
#include "RPG_Systems/Mover/RPG_BaseMoverCharacter.h"

FTransitionEvalResult URagdollTransition::Evaluate_Implementation(const FSimulationTickParams& Params) const
{

	
	FTransitionEvalResult EvalResult = FTransitionEvalResult::NoTransition;
	if (FRagdollAbilityInputs* AbilityInputs = Params.StartState.InputCmd.InputCollection.FindMutableDataByType<FRagdollAbilityInputs>())
	{
		if (AbilityInputs->bWantsToRagdoll)
		{
			 
			auto Character = Cast<ARPG_BaseMoverCharacter>(Params.MovingComps.UpdatedComponent->GetOwner());
			if(!Character || !Character->bCanRagdoll) 	return EvalResult;

			if (Params.StartState.SyncState.MovementMode != "Ragdoll")
			{
				EvalResult.NextMode = "Ragdoll";

			}
			else
			{
				EvalResult.NextMode = DefaultModeNames::Falling;
			}
		} 
	}  
	return EvalResult;
}

void URagdollTransition::Trigger_Implementation(const FSimulationTickParams& Params)
{   

}
 

 