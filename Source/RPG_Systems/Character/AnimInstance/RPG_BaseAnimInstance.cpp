// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Character/AnimInstance/RPG_BaseAnimInstance.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Systems/Character/RPG_CharacterDataAsset.h"


void URPG_BaseAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	RefCharacter = Cast<ARPG_BaseCharacter>(TryGetPawnOwner());

}

void URPG_BaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!RefCharacter)
	{
		return;
	}

	if (!bUseCPP)
	{
		return;
	}
	
	bIsDead = RefCharacter->bIsDead;
	bIsDrinking = RefCharacter->bIsDrinking;
	bIsEating = RefCharacter->bIsEating;
	bIsStunned = RefCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(RefCharacter->StunTag);
	bIsFalling = RefCharacter->GetCharacterMovement()->IsFalling();
	bIsSwimming = RefCharacter->GetCharacterMovement()->IsSwimming();
	bIsJumping = RefCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling ? true : false;
	bIsFlying = RefCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying ? true : false;
	bIsOnGround = RefCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking ? true : false;
	bIsDedicatedServer = GetWorld()->GetNetMode() == NM_DedicatedServer ? true : false;
	FallingVelocity = RefCharacter->GetVelocity().Z;
	MoveSpeed = FVector(RefCharacter->GetVelocity().X, RefCharacter->GetVelocity().Y, 0.f).Size();
	Direction = FMath::FInterpTo(Direction, RefCharacter->GetCapsuleComponent()->GetPhysicsAngularVelocityInDegrees().Z, DeltaSeconds, RotationSpeed);
	ForwardVelocity = UKismetMathLibrary::LessLess_VectorRotator(RefCharacter->GetVelocity(), RefCharacter->GetActorRotation()).X;
	float L_Rate;

	bIsMoving = MoveSpeed >= 10 ? true : false;
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
		
	
}

