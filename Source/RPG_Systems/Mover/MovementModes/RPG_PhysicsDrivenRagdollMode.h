// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"
#include "RPG_PhysicsDrivenRagdollMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_PhysicsDrivenRagdollMode : public URPG_BaseMovementMode
{
	GENERATED_BODY()
public:
	virtual void OnGenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
		FProposedMove& OutProposedMove) const override;
	virtual void OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;


	
};
