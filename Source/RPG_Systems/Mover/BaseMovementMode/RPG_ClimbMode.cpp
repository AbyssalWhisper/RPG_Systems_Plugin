#include "RPG_ClimbMode.h"

#include "BetterUtilitiesBPLibrary.h"
#include "DrawDebugHelpers.h"
#include "ChaosMover/ChaosMoverSimulation.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Systems/Mover/MoverFunctionLibrary.h"
#include "RPG_Systems/Mover/Transitions/RPG_BaseMovementModeTransition.h"

void URPG_ClimbMode::SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState)
{ 
	 
	
	if (!Simulation)
	{
			UBetterUtilities::DebugLog("No Simulation set on UChaosFlyingMode. Check you have a Chaos Backend");
		return;
	}

	const FChaosMoverSimulationDefaultInputs* DefaultSimInputs = Simulation->GetLocalSimInput().FindDataByType<FChaosMoverSimulationDefaultInputs>();
	if (!DefaultSimInputs)
	{
			UBetterUtilities::DebugLog("ChaosFlyingMode requires FChaosMoverSimulationDefaultInputs");
		return;
	}

	const FMoverDefaultSyncState* StartingSyncState = Params.StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	if (!StartingSyncState)
	{
		UBetterUtilities::DebugLog("RPG_ClimbMode requires FMoverDefaultSyncState");
		return;
	}

	
	const FVector ActorLocation = StartingSyncState->GetLocation_WorldSpace();
	const FRotator ActorOrientation = StartingSyncState->GetOrientation_WorldSpace();
	const FVector ActorForward = ActorOrientation.Vector();
	const FVector ActorRight = FRotationMatrix(ActorOrientation).GetUnitAxis(EAxis::Y);
	const FVector ActorUp = GetMoverComponent()->GetUpDirection();
	FVector Input;
	if (const FRPG_RawInput* RawInput = Params.StartState.InputCmd.InputCollection.FindDataByType<FRPG_RawInput>())
	{
		Input = RawInput->MoveInput;
	} 
	InputDirection = ActorRight * Input.Y + ActorUp * Input.X;
	const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;
	FVector FinalNormal;
	
	UCapsuleComponent* Capsule = GetMoverComponent()->GetOwner()->FindComponentByClass<UCapsuleComponent>();
	float Radius = Capsule ? Capsule->GetScaledCapsuleRadius() : 50.0f;
	float Height = Capsule ? Capsule->GetScaledCapsuleHalfHeight() : 100.0f;
	
	float WallTraceDistance = WallOfSet + 20;
	
	
	FHitResult HitResult;
	URPG_BaseMovementModeTransition::SphereTraceSingle(
		GetMoverComponent()->GetOwner(),
		ActorLocation,
		ActorLocation + ActorForward * WallTraceDistance,
		Radius,
		TraceChannel,true,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true);
	
	DrawDebugSphere(GetWorld(), ActorLocation + ActorForward * WallTraceDistance, Radius, 12, FColor::Blue, false, 1.0f);
	DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + ActorForward * WallTraceDistance, FColor::Blue, false, 1.0f);
	DrawDebugSphere(GetWorld(), ActorLocation, Radius, 12, FColor::Blue, false, 1.0f);
	
	if (HitResult.bBlockingHit)
	{
		FinalNormal = HitResult.Normal;
		UBetterUtilities::DebugLog("RPG_ClimbMode: wall detected. Normal: " + FinalNormal.ToString());
		
		FHitResult HitResult2;
		URPG_BaseMovementModeTransition::CapsuleTraceSingle(
			GetMoverComponent()->GetOwner(),
			ActorLocation,
			ActorLocation + InputDirection * WallTraceDistance + ActorForward * WallTraceDistance,
			Radius,Height,
			ActorOrientation,
			TraceChannel,
			true,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			HitResult2,
			true);
		if (HitResult2.bBlockingHit)
		{
			FinalNormal += HitResult2.Normal;
			FinalNormal.Normalize();
			UBetterUtilities::DebugLog("RPG_ClimbMode: blended wall normal: " + FinalNormal.ToString());
		}
		/*
		GetMoverComponent()->SetPlanarConstraint(FPlanarConstraint(
			true,
			HitResult.Location + FinalNormal * 5,
			FinalNormal));*/
		UBetterUtilities::DebugLog("RPG_ClimbMode: planar constraint updated.");
	}
	else
	{
		UBetterUtilities::DebugLog("RPG_ClimbMode: no wall found ahead. Switching to Falling.");
		SwitchToState(FName("Falling"), Params, OutputState);
		return;
	}

	URPG_BaseMovementModeTransition::CapsuleTraceSingle(
			GetMoverComponent()->GetOwner(),
			ActorLocation,
			ActorLocation + InputDirection * WallTraceDistance + ActorForward * WallTraceDistance,
			Radius,Height,
			ActorOrientation,
			TraceChannel,true,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			HitResult,
			true);
	FVector ProjectionPoint;
	if (HitResult.bBlockingHit)
	{
		ProjectionPoint = HitResult.Location + (FinalNormal * WallOfSet);
		UBetterUtilities::DebugLog("RPG_ClimbMode: wall anchor point: " + ProjectionPoint.ToString());
	}else
	{
		UBetterUtilities::DebugLog("RPG_ClimbMode: climb projection trace missed. Switching to Falling.");
		SwitchToState(FName("Falling"), Params, OutputState);
		return;
	} 
	
	float WallAngle = UKismetMathLibrary::Conv_VectorToRotator(-FinalNormal).Pitch;
	UBetterUtilities::DebugLog("RPG_ClimbMode: wall angle: " + FString::SanitizeFloat(WallAngle));
	if (WallAngle >= MaxClimbAngle)
	{
		UBetterUtilities::DebugLog("RPG_ClimbMode: wall angle above limit. Switching to Falling.");
		SwitchToState(FName("Falling"), Params, OutputState);
		return;
	}
	if (WallAngle <= -MaxClimbAngle)
	{
		UBetterUtilities::DebugLog("RPG_ClimbMode: wall angle below limit. Switching to Falling.");
		SwitchToState(FName("Falling"), Params, OutputState);
		return;
	}
	if (GetCurrentFloor().bWalkableFloor)
	{
		UBetterUtilities::DebugLog("RPG_ClimbMode: walkable floor detected. Switching to Falling.");
		SwitchToState(FName("Falling"), Params, OutputState);
		return;
	}
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
	OutputSyncState = *StartingSyncState;

	FProposedMove ProposedMove = Params.ProposedMove;

	const FRotator TargetOrient = UMovementUtils::ApplyAngularVelocityToRotator(StartingSyncState->GetOrientation_WorldSpace(), ProposedMove.AngularVelocityDegrees, DeltaSeconds);

	// The physics simulation applies Z-only gravity acceleration via physics volumes, so we need to account for it here 
	const FVector BaseTargetVel = ProposedMove.LinearVelocity - DefaultSimInputs->PhysicsObjectGravity * FVector::UpVector * DeltaSeconds;
	const FVector PredictedBasePos = StartingSyncState->GetLocation_WorldSpace() + BaseTargetVel * DeltaSeconds;
	const FVector ProjectedTargetPos = UKismetMathLibrary::ProjectPointOnToPlane(
		PredictedBasePos,
		ProjectionPoint,
		FinalNormal);
	const FVector CorrectionVelocity = DeltaSeconds > UE_KINDA_SMALL_NUMBER
		? (ProjectedTargetPos - PredictedBasePos) / DeltaSeconds
		: FVector::ZeroVector;
	FVector TargetVel = BaseTargetVel + CorrectionVelocity;
	FVector TargetPos = StartingSyncState->GetLocation_WorldSpace() + TargetVel * DeltaSeconds;
	DrawDebugSphere(GetWorld(), TargetPos, 10.0f, 12, FColor::Red, false, 1.0f);
	UBetterUtilities::DebugLog("RPG_ClimbMode: base target vel: " + BaseTargetVel.ToString());
	UBetterUtilities::DebugLog("RPG_ClimbMode: projected target pos: " + ProjectedTargetPos.ToString());
	UBetterUtilities::DebugLog("RPG_ClimbMode: correction vel: " + CorrectionVelocity.ToString());
	UBetterUtilities::DebugLog("RPG_ClimbMode: final target vel: " + TargetVel.ToString());

	OutputState.MovementEndState.RemainingMs = 0.0f;
	OutputState.MovementEndState.NextModeName = Params.StartState.SyncState.MovementMode;
	OutputSyncState.MoveDirectionIntent = ProposedMove.bHasDirIntent ? ProposedMove.DirectionIntent : FVector::ZeroVector;
	OutputSyncState.SetTransforms_WorldSpace(
		TargetPos,
		TargetOrient,
		TargetVel,
		ProposedMove.AngularVelocityDegrees);

	NormalResult = FinalNormal;
	UBetterUtilities::DebugLog("RPG_ClimbMode: tick finished. NormalResult: " + NormalResult.ToString());

}
