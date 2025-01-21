// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/MovementModes/RPG_PhysicsDrivenCrouchMode.h"
#include "Components/CapsuleComponent.h"

void URPG_PhysicsDrivenCrouchMode::SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	Super::SimulationTick_Implementation(Params, OutputState);

	const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;
	//return;
	//AjustCapsule(Params, DeltaSeconds);
	
}

float URPG_PhysicsDrivenCrouchMode::GetCapsuleHeight_Implementation()
{
	return TargetCapsuleHalfHeight;
}

float URPG_PhysicsDrivenCrouchMode::GetCapsuleRadius_Implementation()
{
	return TargetCapsuleRadius;

}


