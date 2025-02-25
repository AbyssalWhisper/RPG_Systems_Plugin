// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsMover/Modes/PhysicsDrivenFlyingMode.h"
#include "RPG_PhysicsDrivenClimbingMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_PhysicsDrivenClimbingMode : public UPhysicsDrivenFlyingMode
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float TraceDistance = 100.0f;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float WallAlignmentSpeed = 75.0f;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceType = ETraceTypeQuery::TraceTypeQuery1;
	
	virtual void GenerateMove_Implementation(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
		FProposedMove& OutProposedMove) const override;
	virtual void
	SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;
};
