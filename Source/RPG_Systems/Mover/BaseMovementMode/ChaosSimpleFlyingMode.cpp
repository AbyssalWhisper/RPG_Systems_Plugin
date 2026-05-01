// Copyright EtherCore. All Rights Reserved.

#include "ChaosSimpleFlyingMode.h"

#include "ChaosMover/ChaosMoverLog.h"
#include "ChaosMover/ChaosMoverSimulation.h"
#include "ChaosMover/Character/Settings/SharedChaosCharacterMovementSettings.h"
#include "MoveLibrary/MovementUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ChaosSimpleFlyingMode)

UChaosSimpleFlyingMode::UChaosSimpleFlyingMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UChaosSimpleFlyingMode::GenerateMove_Implementation(const FMoverSimContext& SimContext, const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
{
	QUICK_SCOPE_CYCLE_COUNTER(ChaosSimpleFlyingMode_GenerateMove);
	(void)SimContext;

	if (!ensureMsgf(Simulation, TEXT("No Simulation set on UChaosSimpleFlyingMode. Check you have a Chaos Backend")))
		return;

	const FChaosMoverSimulationDefaultInputs* DefaultSimInputs = Simulation->GetLocalSimInput().FindDataByType<FChaosMoverSimulationDefaultInputs>();
	if (!ensureMsgf(DefaultSimInputs, TEXT("UChaosSimpleFlyingMode requires FChaosMoverSimulationDefaultInputs")))
		return;

	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	if (!ensureMsgf(StartingSyncState, TEXT("UChaosSimpleFlyingMode requires FMoverDefaultSyncState")))
		return;

	const float DeltaSeconds = TimeStep.StepMs * 0.001f;
	if (DeltaSeconds <= FLT_EPSILON)
		return;

	const float EffectiveMaxSpeed = MaxSpeedOverride >= 0.0f ? MaxSpeedOverride : GetMaxSpeed();

	// --- Inputs ---
	FVector DesiredVelocity;
	EMoveInputType MoveInputType;

	if (const FCharacterDefaultInputs* CharacterInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>())
	{
		DesiredVelocity = CharacterInputs->GetMoveInput_WorldSpace();
		MoveInputType   = CharacterInputs->GetMoveInputType();
	}
	else
	{
		DesiredVelocity = StartingSyncState->GetIntent_WorldSpace();
		MoveInputType   = EMoveInputType::DirectionalIntent;
	}

	// --- Scale velocity ---
	switch (MoveInputType)
	{
	case EMoveInputType::DirectionalIntent:
		OutProposedMove.DirectionIntent = DesiredVelocity;
		DesiredVelocity *= EffectiveMaxSpeed;
		break;
	case EMoveInputType::Velocity:
		DesiredVelocity = DesiredVelocity.GetClampedToMaxSize(EffectiveMaxSpeed);
		OutProposedMove.DirectionIntent = EffectiveMaxSpeed > UE_KINDA_SMALL_NUMBER
			? DesiredVelocity / EffectiveMaxSpeed : FVector::ZeroVector;
		break;
	default:
		DesiredVelocity             = FVector::ZeroVector;
		OutProposedMove.DirectionIntent = FVector::ZeroVector;
		break;
	}

	OutProposedMove.bHasDirIntent = !OutProposedMove.DirectionIntent.IsNearlyZero();

	// --- Hook ---
	const FQuat CurrentFacing = StartingSyncState->GetOrientation_WorldSpace().Quaternion();
	FVector FacingDirection = CurrentFacing.GetForwardVector(); // default: keep current facing
	OutProposedMove.LinearVelocity = StartingSyncState->GetVelocity_WorldSpace();

	UChaosSimpleFlyingMode* MutableThis = const_cast<UChaosSimpleFlyingMode*>(this);
	MutableThis->GenerateFlyingMove(
		const_cast<FMoverTickStartData&>(StartState),
		DeltaSeconds,
		DesiredVelocity,
		CurrentFacing,
		FacingDirection,
		OutProposedMove.LinearVelocity);

	// --- Convert facing direction → angular velocity ---
	const FVector SafeFacingDir = FacingDirection.GetSafeNormal(UE_KINDA_SMALL_NUMBER, CurrentFacing.GetForwardVector());
	const FQuat TargetFacing = FQuat::FindBetween(FVector::ForwardVector, SafeFacingDir);
	const FQuat ToFacing = CurrentFacing.Inverse() * TargetFacing;
	OutProposedMove.AngularVelocityDegrees = DeltaSeconds > 0.0f
		? FMath::RadiansToDegrees(ToFacing.ToRotationVector() / DeltaSeconds)
		: FVector::ZeroVector;

	// --- Clear floor data ---
	if (UMoverBlackboard* SimBlackboard = Simulation->GetBlackboard_Mutable())
	{
		SimBlackboard->Invalidate(CommonBlackboard::LastFloorResult);
		SimBlackboard->Invalidate(CommonBlackboard::LastWaterResult);
		SimBlackboard->Invalidate(UE::ChaosMover::Blackboard::GroundDynamicsInfo);
	}
}

void UChaosSimpleFlyingMode::GenerateFlyingMove_Implementation(FMoverTickStartData& StartState, float DeltaSeconds,
	const FVector& DesiredVelocity, const FQuat& CurrentFacing,
	FVector& InOutFacingDirection, FVector& InOutVelocity)
{
	InOutVelocity = DesiredVelocity;
	// InOutFacingDirection default = CurrentFacing.GetForwardVector() → sem rotação
}
