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
	FRotator FacingRotation = CurrentFacing.Rotator(); // default: keep current rotation (all axes)
	OutProposedMove.LinearVelocity = StartingSyncState->GetVelocity_WorldSpace();

	UChaosSimpleFlyingMode* MutableThis = const_cast<UChaosSimpleFlyingMode*>(this);
	MutableThis->GenerateFlyingMove(
		const_cast<FMoverTickStartData&>(StartState),
		DeltaSeconds,
		DesiredVelocity,
		CurrentFacing,
		FacingRotation,
		OutProposedMove.LinearVelocity);

	// --- Convert target rotator → angular velocity (world-space, full 3-axis) ---
	const FQuat TargetFacing = FacingRotation.Quaternion();
	FQuat ToFacing = TargetFacing * CurrentFacing.Inverse();
	ToFacing.Normalize();
	// Garante o caminho curto (<180°): W negativo = rotação pelo lado longo → flip
	if (ToFacing.W < 0.0f)
	{
		ToFacing = FQuat(-ToFacing.X, -ToFacing.Y, -ToFacing.Z, -ToFacing.W);
	}
	OutProposedMove.AngularVelocityDegrees = DeltaSeconds > 0.0f
		? FMath::RadiansToDegrees(ToFacing.ToRotationVector() / DeltaSeconds)
		: FVector::ZeroVector;
	OutProposedMove.MixMode = EMoveMixMode::OverrideAll;
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
	FRotator& InOutFacingRotation, FVector& InOutVelocity)
{
	InOutVelocity = DesiredVelocity;
	// InOutFacingRotation default = CurrentFacing.Rotator() → sem rotação (mantém orientação atual)
}
