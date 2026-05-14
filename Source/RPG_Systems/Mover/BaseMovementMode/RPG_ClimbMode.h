

#pragma once

#include "CoreMinimal.h"
#include "RPG_BaseMovementMode.h"
#include "RPG_ClimbMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_ClimbMode : public URPG_BaseMovementMode
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WallOfSet = 50;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceDistance = 50;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxClimbAngle = 65;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector NormalResult = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector InputDirection = FVector::ZeroVector;
	virtual void
	SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
};
