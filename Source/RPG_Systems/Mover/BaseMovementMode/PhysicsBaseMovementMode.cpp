// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/BaseMovementMode/PhysicsBaseMovementMode.h"

#include "Chaos/Character/CharacterGroundConstraint.h"
#include "Chaos/Character/CharacterGroundConstraintContainer.h"
#include "Chaos/PhysicsObject.h"
#include "Chaos/PhysicsObjectInternalInterface.h"

#include "MoverComponent.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnitConversion.h"
#include "MoveLibrary/AirMovementUtils.h"
#include "PhysicsMover/PhysicsMoverSimulationTypes.h" 
#include "HAL/IConsoleManager.h"

#if WITH_EDITOR
#include "Backends/MoverNetworkPhysicsLiaison.h"
#include "Internationalization/Text.h"
#include "Misc/DataValidation.h"
#endif // WITH_EDITOR


#if PHYSICSDRIVENMOTION_DEBUG_DRAW
#include "Chaos/DebugDrawQueue.h"
#endif
 
#define LOCTEXT_NAMESPACE "BasePhysicsDrivenMode"


void UPhysicsBaseMovementMode::UpdateConstraintSettings(Chaos::FCharacterGroundConstraint& Constraint) const
{
	Constraint.SetRadialForceLimit(300000.0); // TEMP - Move radial force limit to shared mode data
	Constraint.SetTwistTorqueLimit(FUnitConversion::Convert(TwistTorqueLimit, EUnit::NewtonMeters, EUnit::KilogramCentimetersSquaredPerSecondSquared));
	Constraint.SetSwingTorqueLimit(FUnitConversion::Convert(SwingTorqueLimit, EUnit::NewtonMeters, EUnit::KilogramCentimetersSquaredPerSecondSquared));
	Constraint.SetTargetHeight(0.0f);
}
#if WITH_EDITOR
EDataValidationResult UPhysicsBaseMovementMode::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!GetOuter())return Result;
	auto Mover = Cast<UMoverComponent>(GetOuter());
	if (!Mover)return Result;
	const UClass* BackendClass = GetMoverComponent()->BackendClass;
	if (BackendClass && !BackendClass->IsChildOf<UMoverNetworkPhysicsLiaisonComponent>())
	{
		Context.AddError(LOCTEXT("PhysicsMovementModeHasValidPhysicsLiaison", "Physics movement modes need to have a backend class that supports physics (UMoverNetworkPhysicsLiaisonComponent)."));
		Result = EDataValidationResult::Invalid;
	}
		
	return Result;
}
#endif // WITH_EDITOR

void UPhysicsBaseMovementMode::OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{
	const FMoverTickStartData& StartState = Params.StartState;
	auto UpdatedComponent = Params.UpdatedComponent;
	auto UpdatedPrimitive = Params.UpdatedPrimitive;
	FProposedMove ProposedMove = Params.ProposedMove;

	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	check(StartingSyncState);

	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();

	const float DeltaSeconds = GetDeltaSecondsFromTimeStep(Params.TimeStep);
	const FVector UpDir = GetMoverComponent()->GetUpDirection();

	/*
	// Instantaneous movement changes that are executed and we exit before consuming any time
	if (ProposedMove.bHasTargetLocation && AttemptTeleport(UpdatedComponent, ProposedMove.TargetLocation, UpdatedComponent->GetComponentRotation(), *StartingSyncState, OutputState))
	{
		OutputState.MovementEndState.RemainingMs = Params.TimeStep.StepMs; 	// Give back all the time
		return;
	}
*/
	// Don't need a floor query - just invalidate the blackboard to ensure we don't use an old result elsewhere

	if (UMoverBlackboard* SimBlackboard = GetBlackboard_Mutable())
	{
		SimBlackboard->Invalidate(CommonBlackboard::LastFloorResult);
		SimBlackboard->Invalidate(CommonBlackboard::LastWaterResult);
	}

	// In air steering

	FRotator TargetOrient = StartingSyncState->GetOrientation_WorldSpace();
	if (!ProposedMove.AngularVelocity.IsZero())
	{
		TargetOrient += (ProposedMove.AngularVelocity * DeltaSeconds);
	}

	FVector TargetVel = ProposedMove.LinearVelocity;
	if (const APhysicsVolume* CurPhysVolume = UpdatedComponent->GetPhysicsVolume())
	{
		// The physics simulation applies Z-only gravity acceleration via physics volumes, so we need to account for it here 
		TargetVel -= (CurPhysVolume->GetGravityZ() * FVector::UpVector * DeltaSeconds);
	}

	FVector TargetPos = StartingSyncState->GetLocation_WorldSpace() + TargetVel * DeltaSeconds;

	OutputState.MovementEndState.NextModeName = NextModeName;

	OutputState.MovementEndState.RemainingMs = 0.0f;
	OutputSyncState.MoveDirectionIntent = ProposedMove.bHasDirIntent ? ProposedMove.DirectionIntent : FVector::ZeroVector;
	OutputSyncState.SetTransforms_WorldSpace(
		TargetPos,
		TargetOrient,
		TargetVel);
}

void UPhysicsBaseMovementMode::OnGenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
	FProposedMove& OutProposedMove) const
{
	const FCharacterDefaultInputs* CharacterInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	check(StartingSyncState);

	const float DeltaSeconds = GetDeltaSecondsFromTimeStep(TimeStep);

	FFreeMoveParams Params;
	if (CharacterInputs)
	{
		Params.MoveInputType = CharacterInputs->GetMoveInputType();
		Params.MoveInput = CharacterInputs->GetMoveInput();
	}
	else
	{
		Params.MoveInputType = EMoveInputType::Invalid;
		Params.MoveInput = FVector::ZeroVector;
	}
 
	FRotator IntendedOrientation_WorldSpace;
	
	// If there's no intent from input to change orientation, use the current orientation
	if (!CharacterInputs || CharacterInputs->OrientationIntent.IsNearlyZero())
	{
		IntendedOrientation_WorldSpace = StartingSyncState->GetOrientation_WorldSpace();
	}
	else
	{
		IntendedOrientation_WorldSpace = CharacterInputs->GetOrientationIntentDir_WorldSpace().ToOrientationRotator();
	}
	
	Params.OrientationIntent = IntendedOrientation_WorldSpace;
	Params.PriorVelocity = StartingSyncState->GetVelocity_WorldSpace();
	Params.PriorOrientation = StartingSyncState->GetOrientation_WorldSpace();
	Params.TurningRate = CommonLegacySettings->TurningRate;
	Params.TurningBoost = CommonLegacySettings->TurningBoost;
	Params.MaxSpeed = CommonLegacySettings->MaxSpeed;
	Params.Acceleration = CommonLegacySettings->Acceleration;
	Params.Deceleration = CommonLegacySettings->Deceleration;
	Params.DeltaSeconds = DeltaSeconds;

	OutProposedMove = UAirMovementUtils::ComputeControlledFreeMove(Params);
}

void UPhysicsBaseMovementMode::OnRegistered(const FName ModeName)
{
	Super::OnRegistered(ModeName);
	CommonLegacySettings = GetMoverComponent()->FindSharedSettings<UCommonLegacyMovementSettings>();
	ensureMsgf(CommonLegacySettings, TEXT("Failed to find instance of CommonLegacyMovementSettings on %s. Movement may not function properly."), *GetPathNameSafe(this));

}

void UPhysicsBaseMovementMode::OnUnregistered()
{
	CommonLegacySettings = nullptr;
	Super::OnUnregistered();
}

float UPhysicsBaseMovementMode::GetDeltaSecondsFromTimeStep(const FMoverTimeStep& TimeStep) const
{
	return TimeStep.StepMs * 0.001f;
}

void UPhysicsBaseMovementMode::DrawCapsule(FVector Start,FVector End,float QueryRadius)
{
 
	const FVector Center = 0.5f * (Start + End);
	const float Dist = (Start - End).Size();
	Chaos::FDebugDrawQueue::GetInstance().DrawDebugCapsule(Center, 0.5f * Dist + QueryRadius, QueryRadius, FQuat::Identity, FColor::Silver, false, -1.f, 10, 1.0f);
	
}


void UPhysicsBaseMovementMode::SwitchToState(const FName& StateName, const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
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