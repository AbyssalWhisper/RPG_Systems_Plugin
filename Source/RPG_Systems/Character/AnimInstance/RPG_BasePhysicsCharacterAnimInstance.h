// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoverSimulationTypes.h"
#include "Animation/AnimInstance.h"
#include "RPG_Systems/Character/RPG_CharacterLibrary.h"
#include "RPG_Systems/Character/RPG_CharacterDataAsset.h"
#include "RPG_BasePhysicsCharacterAnimInstance.generated.h"
class ARPG_BasePhysicsCharacter;
/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_BasePhysicsCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void GetCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ARPG_BaseMoverCharacter* RefCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseScalePlayRate = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FallingVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ForwardVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkRunBlend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StandingPlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPlayRate = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUseCPP = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsEating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDrinking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMoving;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float IsMovingStartVelocity = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsStunned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSwimming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsFlying;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsOnGround;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDedicatedServer;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FTransformTrajectory GetTrajectory(FMoverPredictTrajectoryParams TrajectoryParams);
};
