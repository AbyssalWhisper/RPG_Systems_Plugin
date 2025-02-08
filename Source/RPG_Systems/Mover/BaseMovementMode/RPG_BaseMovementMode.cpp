// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"

#include "Chaos/PhysicsObject.h"

#include "MoverComponent.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "MoveLibrary/AirMovementUtils.h"
#include "PhysicsMover/PhysicsMoverSimulationTypes.h" 
#include "HAL/IConsoleManager.h"
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

URPG_BaseMovementMode::URPG_BaseMovementMode()
{ 
}
#if WITH_EDITOR
EDataValidationResult URPG_BaseMovementMode::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	return Result;
}
#endif // WITH_EDITOR

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 5
void URPG_BaseMovementMode::OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
void URPG_BaseMovementMode::SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
#endif
{
 
const UMoverComponent* MoverComp = GetMoverComponent();
	const FMoverTickStartData& StartState = Params.StartState;
	USceneComponent* UpdatedComponent = Params.MovingComps.UpdatedComponent.Get();
	FProposedMove ProposedMove = Params.ProposedMove;

	const FCharacterDefaultInputs* CharacterInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	check(StartingSyncState);

	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();

	const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;

	FMovementRecord MoveRecord;
	MoveRecord.SetDeltaSeconds(DeltaSeconds);

	UMoverBlackboard* SimBlackboard = MoverComp->GetSimBlackboard_Mutable();

	SimBlackboard->Invalidate(CommonBlackboard::LastFloorResult);	// flying = no valid floor
	SimBlackboard->Invalidate(CommonBlackboard::LastFoundDynamicMovementBase);

	OutputSyncState.MoveDirectionIntent = (ProposedMove.bHasDirIntent ? ProposedMove.DirectionIntent : FVector::ZeroVector);

	// Use the orientation intent directly. If no intent is provided, use last frame's orientation. Note that we are assuming rotation changes can't fail. 
	const FRotator StartingOrient = StartingSyncState->GetOrientation_WorldSpace();
	FRotator TargetOrient = StartingOrient;

	bool bIsOrientationChanging = false;

	// Apply orientation changes (if any)
	if (!UMovementUtils::IsAngularVelocityZero(ProposedMove.AngularVelocity))
	{
		TargetOrient += (ProposedMove.AngularVelocity * DeltaSeconds);
		bIsOrientationChanging = (TargetOrient != StartingOrient);
	}
	
	FVector MoveDelta = ProposedMove.LinearVelocity * DeltaSeconds;
	const FQuat OrientQuat = TargetOrient.Quaternion();
	FHitResult Hit(1.f);

	if (!MoveDelta.IsNearlyZero() || bIsOrientationChanging)
	{
		UMovementUtils::TrySafeMoveUpdatedComponent(Params.MovingComps, MoveDelta, OrientQuat, true, Hit, ETeleportType::None, MoveRecord);
	}

	if (Hit.IsValidBlockingHit())
	{
		UMoverComponent* MoverComponent = GetMoverComponent();
		FMoverOnImpactParams ImpactParams(DefaultModeNames::Flying, Hit, MoveDelta);
		MoverComponent->HandleImpact(ImpactParams);
		// Try to slide the remaining distance along the surface.
		UMovementUtils::TryMoveToSlideAlongSurface(FMovingComponentSet(MoverComponent), MoveDelta, 1.f - Hit.Time, OrientQuat, Hit.Normal, Hit, true, MoveRecord);
	}

	CaptureFinalState(UpdatedComponent, MoveRecord, *StartingSyncState, OutputSyncState, DeltaSeconds);

}

void URPG_BaseMovementMode::CaptureFinalState(USceneComponent* UpdatedComponent, FMovementRecord& Record,
	const FMoverDefaultSyncState& StartSyncState, FMoverDefaultSyncState& OutputSyncState,
	const float DeltaSeconds) const
{
	const FVector FinalLocation = UpdatedComponent->GetComponentLocation();
	const FVector FinalVelocity = Record.GetRelevantVelocity();
	
	// TODO: Update Main/large movement record with substeps from our local record

	OutputSyncState.SetTransforms_WorldSpace(FinalLocation,
											  UpdatedComponent->GetComponentRotation(),
											  FinalVelocity,
											  nullptr); // no movement base

	UpdatedComponent->ComponentVelocity = FinalVelocity;
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 5
void URPG_BaseMovementMode::OnGenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
void URPG_BaseMovementMode::GenerateMove_Implementation(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
										   FProposedMove& OutProposedMove) const
#endif
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

void URPG_BaseMovementMode::OnRegistered(const FName ModeName)
{
	Super::OnRegistered(ModeName);
	CommonLegacySettings = GetMoverComponent()->FindSharedSettings<UCommonLegacyMovementSettings>();
	ensureMsgf(CommonLegacySettings, TEXT("Failed to find instance of CommonLegacyMovementSettings on %s. Movement may not function properly."), *GetPathNameSafe(this));

}

void URPG_BaseMovementMode::OnUnregistered()
{
	CommonLegacySettings = nullptr;
	Super::OnUnregistered();
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
	
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	Params.WorldToGravityQuat = WorldToGravityQuat;
	Params.bUseAccelerationForVelocityMove = bUseAccelerationForVelocityMove;
#endif
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

FFloorCheckResult URPG_BaseMovementMode::GetCurrentFloor() const
{
	FFloorCheckResult CurrentFloor;
	UMoverBlackboard* SimBlackboard = GetMoverComponent()->GetSimBlackboard_Mutable();

	const FVector UpDirection = GetMoverComponent()->GetUpDirection();
	FMovingComponentSet MovingComponents(GetMoverComponent());
	
	// If we don't have cached floor information, we need to search for it again
	if (!SimBlackboard->TryGet(CommonBlackboard::LastFloorResult, CurrentFloor))
	{
	#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 5
		UFloorQueryUtils::FindFloor(MovingComponents.UpdatedComponent.Get()
			,MovingComponents.UpdatedPrimitive.Get(),
			CommonLegacySettings->FloorSweepDistance,
			CommonLegacySettings->MaxWalkSlopeCosine,
			GetMoverComponent()->GetUpdatedComponent()->GetComponentLocation(),
			CurrentFloor);
	#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
				UFloorQueryUtils::FindFloor(MovingComponents, CommonLegacySettings->FloorSweepDistance, CommonLegacySettings->MaxWalkSlopeCosine, GetMoverComponent()->GetUpdatedComponent()->GetComponentLocation(), CurrentFloor);
		#endif

	}
	return CurrentFloor;
}

FVector URPG_BaseMovementMode::ConstrainDirectionToPlane(const FMoverTickStartData& StartState, bool bMaintainMagnitude) const
{
	const FCharacterDefaultInputs* CharacterInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	if (!CharacterInputs)return FVector::ZeroVector;
	return UPlanarConstraintUtils::ConstrainDirectionToPlane(GetMoverComponent()->GetPlanarConstraint(), CharacterInputs->GetMoveInput_WorldSpace(), bMaintainMagnitude);
}
