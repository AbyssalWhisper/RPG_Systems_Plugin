// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/MovementModes/RPG_PhysicsDrivenRagdollMode.h"

#include "DefaultMovementSet/CharacterMoverComponent.h" 
#include "BetterUtilities/public/BetterUtilitiesBPLibrary.h"
#include "RPG_Systems/Mover/Transitions/RagdollTransition.h"
void URPG_PhysicsDrivenRagdollMode::OnGenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
{

}

void URPG_PhysicsDrivenRagdollMode::OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	if (FRagdollAbilityInputs* AbilityInputs = Params.StartState.InputCmd.InputCollection.FindMutableDataByType<FRagdollAbilityInputs>())
	{
		if (AbilityInputs->bWantsToRagdoll)
		{
			SwitchToState("Walking", Params, OutputState);
		}
	}

}
