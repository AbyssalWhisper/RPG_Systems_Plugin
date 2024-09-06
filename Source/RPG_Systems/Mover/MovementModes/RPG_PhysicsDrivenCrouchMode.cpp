// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/MovementModes/RPG_PhysicsDrivenCrouchMode.h"
#include "Components/CapsuleComponent.h"

void URPG_PhysicsDrivenCrouchMode::OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	Super::OnSimulationTick(Params, OutputState);

	const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;
	//return;
	AjustCapsule(Params, DeltaSeconds);
}

void URPG_PhysicsDrivenCrouchMode::AjustCapsule(const FSimulationTickParams& Params,const float& DeltaSeconds)
{
	auto capsule = Cast<UCapsuleComponent>(Params.MovingComps.UpdatedComponent);
	if (capsule)
	{
		if (capsule->GetUnscaledCapsuleHalfHeight() != TargetCapsuleHalfHeight || capsule->GetUnscaledCapsuleRadius() != TargetCapsuleRadius)
		{
			capsule->SetCapsuleSize(FMath::FInterpTo(capsule->GetUnscaledCapsuleRadius(), TargetCapsuleRadius, DeltaSeconds, 2),
				FMath::FInterpTo(capsule->GetUnscaledCapsuleHalfHeight(), TargetCapsuleHalfHeight, DeltaSeconds, 2));
			
		}
	}
	
}
