// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Character/AnimInstance/RPG_BasePhysicsCharacterAnimInstance.h"

#include "Animation/TrajectoryTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MovementComponent.h"
#include "MoveLibrary/MovementUtils.h"
#include "RPG_Systems/Character/RPG_CharacterDataAsset.h"
#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"
#include "RPG_Systems/Mover/MovementComponents/RPG_CharacterMoverComponent.h"


void URPG_BasePhysicsCharacterAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	if (!GetWorld())return;
	bIsDedicatedServer = GetWorld()->GetNetMode() == NM_DedicatedServer ? true : false;
	
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &URPG_BasePhysicsCharacterAnimInstance::GetCharacter);
}

void URPG_BasePhysicsCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!bUseCPP)return;

	if (!RefCharacter)return;

	//bIsDead = RefCharacter->bIsDead;
	//bIsDrinking = RefCharacter->bIsDrinking;
	//bIsEating = RefCharacter->bIsEating;
	//bIsStunned = RefCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(RefCharacter->StunTag);
	
	bIsFalling = RefCharacter->GetMoverComponent()->IsFalling();
	bIsSwimming = RefCharacter->GetMoverComponent()->IsSwimming();
	bIsJumping = RefCharacter->GetMoverComponent()->IsFalling();
	bIsFlying = RefCharacter->GetMoverComponent()->IsFlying();
	bIsOnGround = RefCharacter->GetMoverComponent()->IsOnGround();
	
	if (bIsFalling) FallingVelocity = RefCharacter->GetMoverComponent()->GetVelocity().Z;

	if (bIsOnGround) {
		MoveSpeed = FVector(RefCharacter->GetMoverComponent()->GetVelocity().X,
			RefCharacter->GetMoverComponent()->GetVelocity().Y, 0.f).Size();
	}
	else
	{
		MoveSpeed = RefCharacter->GetMoverComponent()->GetVelocity().Size();
	}

	bIsMoving = MoveSpeed >= IsMovingStartVelocity;

	//Direction = FMath::FInterpTo(Direction, RefCharacter->GetCapsuleComponent()->GetPhysicsAngularVelocityInDegrees().Z, DeltaSeconds, RotationSpeed);
	ForwardVelocity = UKismetMathLibrary::LessLess_VectorRotator(RefCharacter->GetVelocity(), RefCharacter->GetActorRotation()).X;
	
	/*
	float L_Rate;

	
	float L_AnimSlowWalkMoveSpeed = 0;
	float L_AnimWalkMoveSpeed = 0;
	float L_AnimRunMoveSpeed = 0;
	float L_AnimSprintMoveSpeed = 0;
	
	if (RefCharacter->CharacterData)
	{
		L_AnimSlowWalkMoveSpeed = RefCharacter->CharacterData->AnimSlowWalkMoveSpeed;
		L_AnimWalkMoveSpeed = RefCharacter->CharacterData->AnimWalkMoveSpeed;
		L_AnimRunMoveSpeed = RefCharacter->CharacterData->AnimRunMoveSpeed;
		L_AnimSprintMoveSpeed = RefCharacter->CharacterData->AnimSprintMoveSpeed;
	}

	float ZScale = GetOwningComponent()->GetComponentScale().Z;
		switch (RefCharacter->MovementStates)
		{
		case SlowWalk:
			WalkRunBlend = 0;
			L_Rate = MoveSpeed / L_AnimSlowWalkMoveSpeed;
			break;
		case Walk:
			WalkRunBlend = FMath::GetMappedRangeValueClamped(FVector2D(ZScale * L_AnimSlowWalkMoveSpeed, ZScale * L_AnimWalkMoveSpeed), FVector2D(0.f, 1.f), MoveSpeed);
			L_Rate = MoveSpeed / L_AnimWalkMoveSpeed;
			break;
		case Run:
			WalkRunBlend = FMath::GetMappedRangeValueClamped(FVector2D(ZScale * L_AnimWalkMoveSpeed, ZScale * L_AnimRunMoveSpeed), FVector2D(1.f, 2.f), MoveSpeed);
			L_Rate = MoveSpeed / L_AnimRunMoveSpeed;
			break;
		case Sprint:
			WalkRunBlend = FMath::GetMappedRangeValueClamped(FVector2D(ZScale * L_AnimRunMoveSpeed, ZScale * L_AnimSprintMoveSpeed), FVector2D(2.f, 3.f), MoveSpeed);
			L_Rate = MoveSpeed / L_AnimSprintMoveSpeed;
			break;
		default:
			break;
		}
		if (bUseScalePlayRate)
		{
			StandingPlayRate = FMath::Min(L_Rate / ZScale, MaxPlayRate);
		}
		else
		{
			StandingPlayRate = FMath::Min(L_Rate, MaxPlayRate);
		}
		*/
		
}

void URPG_BasePhysicsCharacterAnimInstance::GetCharacter()
{
	RefCharacter = Cast<ARPG_BaseMoverCharacter>(TryGetPawnOwner());
}

FTransformTrajectory URPG_BasePhysicsCharacterAnimInstance::GetTrajectory(FMoverPredictTrajectoryParams TrajectoryParams)
{
	if (RefCharacter && RefCharacter->GetMoverComponent())
	{
		
		URPG_CharacterMoverComponent* Mover = RefCharacter->GetMoverComponent();
		
		TArray<FTrajectorySampleInfo> TrajectorySampleInfos = RefCharacter->GetMoverComponent()->GetPredictedTrajectory(TrajectoryParams);
		FTransformTrajectory Trajectory;
		Trajectory.Samples.SetNum(TrajectorySampleInfos.Num());
		int Size = TrajectorySampleInfos.Num();
		
		for (int i = 0; i < Size; ++i)
		{
			Trajectory.Samples[i].SetTransform(TrajectorySampleInfos[i].Transform);
			Trajectory.Samples[i].TimeInSeconds = 0.001f * TrajectorySampleInfos[i].SimTimeMs;
		}
		return Trajectory;
	}
	return FTransformTrajectory();
}


