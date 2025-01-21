// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"
#include "PhysicsDrivenClimbMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UPhysicsDrivenClimbMode : public URPG_BaseMovementMode
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;
protected:
	virtual void GenerateMove_Implementation(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
		FProposedMove& OutProposedMove) const override;
};
