// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"

#include "Chaos/PhysicsObject.h"

#include "MoverComponent.h"
#include "Chaos/Character/CharacterGroundConstraintSettings.h"
#include "ChaosMover/ChaosMoverLog.h"
#include "ChaosMover/ChaosMoverSimulation.h"
#include "ChaosMover/ChaosMoverSimulationTypes.h"
#include "ChaosMover/Character/Settings/SharedChaosCharacterMovementSettings.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "GameFramework/PhysicsVolume.h"
#include "MoveLibrary/AirMovementUtils.h"
#include "PhysicsMover/PhysicsMoverSimulationTypes.h" 
#include "HAL/IConsoleManager.h"
#include "Math/UnitConversion.h"
#include "MoveLibrary/MovementUtils.h"

#if WITH_EDITOR
#include "Internationalization/Text.h"
#endif // WITH_EDITOR


#if PHYSICSDRIVENMOTION_DEBUG_DRAW
#endif



/*
void URPG_BaseMovementMode::UpdateConstraintSettings(Chaos::FCharacterGroundConstraint& Constraint) const
{
	Constraint.SetRadialForceLimit(300000.0); // TEMP - Move radial force limit to shared mode data
	Constraint.SetTwistTorqueLimit(FUnitConversion::Convert(TwistTorqueLimit, EUnit::NewtonMeters, EUnit::KilogramCentimetersSquaredPerSecondSquared));
	Constraint.SetSwingTorqueLimit(FUnitConversion::Convert(SwingTorqueLimit, EUnit::NewtonMeters, EUnit::KilogramCentimetersSquaredPerSecondSquared));
	Constraint.SetTargetHeight(0.0f);
}*/

URPG_BaseMovementMode::URPG_BaseMovementMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

#if WITH_EDITOR


EDataValidationResult URPG_BaseMovementMode::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	return Result;
}
#endif // WITH_EDITOR

void URPG_BaseMovementMode::SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	Super::SimulationTick_Implementation(Params, OutputState);
} 

void URPG_BaseMovementMode::OnRegistered(const FName ModeName, const FMoverSimContext& SimContext)
{
	Super::OnRegistered(ModeName, SimContext);
	ChaosCharacterMovementSettings = GetMoverComponent()->FindSharedSettings_Mutable<USharedChaosCharacterMovementSettings>();
}

void URPG_BaseMovementMode::OnUnregistered(const FMoverSimContext& SimContext)
{
	ChaosCharacterMovementSettings = nullptr;
	Super::OnUnregistered(SimContext);
}

 void URPG_BaseMovementMode::GenerateMove_Implementation(const FMoverSimContext& SimContext, const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
										   FProposedMove& OutProposedMove) const
{
	Super::GenerateMove_Implementation(SimContext, StartState, TimeStep, OutProposedMove);
}



float URPG_BaseMovementMode::GetDeltaSecondsFromTimeStep(const FMoverTimeStep& TimeStep) const
{
	return TimeStep.StepMs * 0.001f;
}

void URPG_BaseMovementMode::SwitchToState(const FName& StateName, const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	OutputState.MovementEndState.RemainingMs = Params.TimeStep.StepMs;
	OutputState.MovementEndState.NextModeName = StateName;

	const FMoverDefaultSyncState* StartingSyncState = Params.StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
	OutputSyncState.SetTransforms_WorldSpace(
		StartingSyncState->GetLocation_WorldSpace(),
		StartingSyncState->GetOrientation_WorldSpace(),
		StartingSyncState->GetVelocity_WorldSpace(),
		nullptr);
}

FFreeMoveParams URPG_BaseMovementMode::MakeFreeMoveParams(EMoveInputType MoveInputType, FVector MoveInput,
	FRotator OrientationIntent, FVector PriorVelocity, FRotator PriorOrientation, float MaxSpeed, float Acceleration,
	float Deceleration, float TurningBoost, float TurningRate, float DeltaSeconds, FQuat WorldToGravityQuat,
	bool bUseAccelerationForVelocityMove)
{
	FFreeMoveParams Params;
	Params.MoveInputType = MoveInputType;
	Params.MoveInput = MoveInput;
	Params.OrientationIntent = OrientationIntent;
	Params.PriorVelocity = PriorVelocity;
	Params.PriorOrientation = PriorOrientation;
	Params.MaxSpeed = MaxSpeed;
	Params.Acceleration = Acceleration;
	Params.Deceleration = Deceleration;
	Params.TurningBoost = TurningBoost;
	Params.TurningRate = TurningRate;
	Params.DeltaSeconds = DeltaSeconds;
	 
	Params.WorldToGravityQuat = WorldToGravityQuat;
	Params.bUseAccelerationForVelocityMove = bUseAccelerationForVelocityMove; 
	return Params;
}

FCharacterDefaultInputs& URPG_BaseMovementMode::GetDefaultInputs(const FMoverTickStartData& StartState)
{
	return *StartState.InputCmd.InputCollection.FindMutableDataByType<FCharacterDefaultInputs>();
}

FCharacterDefaultInputs& URPG_BaseMovementMode::GetDefaultInputsFromInputCmd(const FMoverInputCmdContext& InputCmd)
{
	return *InputCmd.InputCollection.FindMutableDataByType<FCharacterDefaultInputs>();
}

FMoverDefaultSyncState& URPG_BaseMovementMode::GetMoverDefaultSyncState(const FMoverTickStartData& StartState)
{
	return *StartState.SyncState.SyncStateCollection.FindMutableDataByType<FMoverDefaultSyncState>();
}

FVector URPG_BaseMovementMode::GetVelocity_WorldSpace(const FMoverDefaultSyncState& MoverDefaultSyncState)
{
	return MoverDefaultSyncState.GetVelocity_WorldSpace();
}

FRotator URPG_BaseMovementMode::GetOrientation_WorldSpace(const FMoverDefaultSyncState& MoverDefaultSyncState)
{
	return MoverDefaultSyncState.GetOrientation_WorldSpace();
}

void URPG_BaseMovementMode::UpdateConstraintSettings(Chaos::FCharacterGroundConstraintSettings& ConstraintSettings) const
{
	Super::UpdateConstraintSettings(ConstraintSettings);
}

FFloorCheckResult URPG_BaseMovementMode::GetCurrentFloor() const
{
	FFloorCheckResult CurrentFloor;
	UMoverBlackboard* SimBlackboard = GetMoverComponent()->GetSimBlackboard_Mutable();
	const FVector UpDirection = GetMoverComponent()->GetUpDirection();
	FMovingComponentSet MovingComponents(GetMoverComponent());
	
	// If we don't have cached floor information, we need to search for it again
	if (!SimBlackboard->TryGet(CommonBlackboard::LastFloorResult, CurrentFloor))
	{
		UFloorQueryUtils::FindFloor(MovingComponents,
				GetTargetHeight(),
				ChaosCharacterMovementSettings->MaxWalkableSlopeAngle,
				true,
				GetMoverComponent()->GetUpdatedComponent()->GetComponentLocation(),
				CurrentFloor);
	}
	return CurrentFloor;
}

