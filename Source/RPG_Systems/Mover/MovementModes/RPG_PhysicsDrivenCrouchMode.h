// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"

#include "RPG_Systems/Mover/MovementModeInterface.h"

#include "RPG_PhysicsDrivenCrouchMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_PhysicsDrivenCrouchMode : public UPhysicsDrivenWalkingMode , public IMovementModeInterface
{
	GENERATED_BODY()
public:
	virtual void OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;
	
	float GetCapsuleHeight_Implementation() override;
	float GetCapsuleRadius_Implementation() override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float TargetCapsuleHalfHeight = 40;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetCapsuleRadius = 40;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CapsuleChangeSpeed = 2;
};
