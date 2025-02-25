// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/MovementModes/RPG_PhysicsDrivenClimbingMode.h"

#include "AsyncTickFunctions.h"
#include "InstantMovementEffect.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/InstantMovementEffects/BasicInstantMovementEffects.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "MoveLibrary/AirMovementUtils.h"
#include "Mover/Public/MoverComponent.h"
#include "RPG_Systems/Mover/MoverFunctionLibrary.h"
#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"
#include "RPG_Systems/Mover/Transitions/RPG_BaseMovementModeTransition.h"


void URPG_PhysicsDrivenClimbingMode::GenerateMove_Implementation(const FMoverTickStartData& StartState,
                                                                 const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
{
	FVector RawInput = UMoverFunctionLibrary::GetRawInput(StartState.InputCmd.InputCollection).MoveInput;
	const FCharacterDefaultInputs* DefaultInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	

	UMoverComponent* Mover = GetMoverComponent<UMoverComponent>();
	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Mover->GetUpdatedComponent());

	FVector CapsuleLocation = Capsule->GetComponentLocation();
	FVector CapsuleForward = Capsule->GetForwardVector();
	FVector CapsuleForwardEnd = Capsule->GetForwardVector() * TraceDistance;
	FVector Normal = FVector::ZeroVector;
	FHitResult Hit;
	
	URPG_BaseMovementModeTransition::SphereTraceSingle(Mover,
		CapsuleLocation, CapsuleLocation + CapsuleForwardEnd,
		Capsule->GetScaledCapsuleRadius(), TraceType,
		false, TArray<AActor*>(), EDrawDebugTrace::None, Hit,true);
	FVector InputDirection = FVector(Capsule->GetUpVector() * RawInput.X + Capsule->GetRightVector() * RawInput.Y);
		

	if (Hit.bBlockingHit)
	{
		Normal = Hit.ImpactNormal;
		FHitResult Hit2;

		
		URPG_BaseMovementModeTransition::CapsuleTraceSingle(Mover,
			CapsuleLocation, CapsuleForward * 10 + InputDirection * 10 + CapsuleLocation,
			Capsule->GetScaledCapsuleRadius(),Capsule->GetScaledCapsuleHalfHeight(),
			Capsule->GetComponentRotation(), TraceType,
			false, TArray<AActor*>(), EDrawDebugTrace::None, Hit2,true);
		if (Hit2.bBlockingHit)
		{
			Normal = (Hit2.Normal + Normal).GetSafeNormal();
		}
		FPlanarConstraint PlanarConstraint;
		PlanarConstraint.bConstrainToPlane = true;
		PlanarConstraint.PlaneConstraintOrigin = Hit.Location + Normal * 5;
		PlanarConstraint.PlaneConstraintNormal = Normal;
		Mover->SetPlanarConstraint(PlanarConstraint);
	}
	else
	{
		OutProposedMove.PreferredMode = "Falling";
		return;
	}
	FHitResult Hit3;
	URPG_BaseMovementModeTransition::CapsuleTraceSingle(Mover,
		CapsuleLocation, CapsuleLocation + CapsuleForwardEnd,
		Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(),
		Capsule->GetComponentRotation(), TraceType,
		false, TArray<AActor*>(), EDrawDebugTrace::None, Hit3,true);

	if (Hit3.bBlockingHit)
	{
		UAsyncTickFunctions::ATP_SetWorldLocation(Capsule,UKismetMathLibrary::ProjectPointOnToPlane(CapsuleLocation, Hit3.Location+ Normal* 5, Normal));
		TSharedPtr<FTeleportEffect> teleport = MakeShared<FTeleportEffect>();
		teleport->TargetLocation = UKismetMathLibrary::ProjectPointOnToPlane(CapsuleLocation, Hit3.Location+ Normal* 5, Normal);
		
		Mover->QueueInstantMovementEffect(teleport);
	}
	else
	{
		
		OutProposedMove.PreferredMode = "Falling";
		return;
	}
	FFloorCheckResult FloorCheck;
	Mover->GetSimBlackboard_Mutable()->TryGet(CommonBlackboard::LastFloorResult, FloorCheck);
	
	FRotator Rotator = UKismetMathLibrary::MakeRotFromX(Normal);
	if (Rotator.Pitch <= -65 || Rotator.Pitch >= 65)
	{
		OutProposedMove.PreferredMode = "Falling";
		return;
	}
	else if (FloorCheck.bWalkableFloor && RawInput.X < -0.2)
	{
		OutProposedMove.PreferredMode = "Falling";
		return;
	}

	FRotator NormalDirection = UKismetMathLibrary::MakeRotFromX(-Normal);
	
	
	bool bShouldUpdateRotation = (Mover->GetVelocity().Length() > 10.f) &&
		!UKismetMathLibrary::NormalizedDeltaRotator(
			NormalDirection,
			StartingSyncState->GetOrientation_WorldSpace()).Equals(StartingSyncState->GetOrientation_WorldSpace(),7); 

	FRotator TargetOrient = bShouldUpdateRotation ? NormalDirection : StartingSyncState->GetOrientation_WorldSpace();
	
	auto FreeMove = URPG_BaseMovementMode::MakeFreeMoveParams(
		DefaultInputs->GetMoveInputType(),
		InputDirection,TargetOrient,StartingSyncState->GetVelocity_WorldSpace(),
		StartingSyncState->GetOrientation_WorldSpace(),CommonLegacySettings->MaxSpeed,
		CommonLegacySettings->Acceleration,
		CommonLegacySettings->Deceleration,0,
		WallAlignmentSpeed,TimeStep.StepMs * 0.001f,
		Mover->GetGravityToWorldTransform(),
		CommonLegacySettings->bUseAccelerationForVelocityMove);
	
	OutProposedMove = UAirMovementUtils::ComputeControlledFreeMove(FreeMove);
	
}

void URPG_PhysicsDrivenClimbingMode::SimulationTick_Implementation(const FSimulationTickParams& Params,
                                                                   FMoverTickEndData& OutputState)
{
	Super::SimulationTick_Implementation(Params, OutputState);
	
	OutputState.MovementEndState.NextModeName = Params.ProposedMove.PreferredMode;
}
