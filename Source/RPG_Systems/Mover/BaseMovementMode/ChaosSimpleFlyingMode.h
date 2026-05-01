// Copyright EtherCore. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChaosMover/Character/Modes/ChaosFlyingMode.h"
#include "ChaosSimpleFlyingMode.generated.h"

/**
 * UChaosSimpleFlyingMode
 *
 * A Chaos-backed flying mode with direct-velocity movement (no acceleration ramp).
 * Override GenerateFlyingMove to control both movement and capsule facing direction.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, meta = (DisplayName = "Chaos Simple Flying Mode"))
class RPG_SYSTEMS_API UChaosSimpleFlyingMode : public UChaosFlyingMode
{
	GENERATED_BODY()

public:
	UChaosSimpleFlyingMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GenerateMove_Implementation(const FMoverSimContext& SimContext, const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const override;

	/**
	 * Override to implement custom flying movement.
	 *
	 * @param StartState           Full tick start data (inputs, sync state, etc.)
	 * @param DeltaSeconds         Frame delta time in seconds
	 * @param DesiredVelocity      Velocity from input, already scaled to MaxSpeed
	 * @param CurrentFacing        Character's current facing quaternion
	 * @param InOutFacingDirection World-space direction the capsule should face.
	 *                             Default: current forward vector (no rotation).
	 *                             Set to any direction to rotate the capsule:
	 *                               FVector::UpVector       → face up
	 *                               -FVector::ForwardVector → face backward
	 * @param InOutVelocity        Linear velocity to apply
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Generate Simple Flying Move"))
	void GenerateFlyingMove(UPARAM(ref) FMoverTickStartData& StartState, float DeltaSeconds,
		const FVector& DesiredVelocity, const FQuat& CurrentFacing,
		UPARAM(ref) FVector& InOutFacingDirection, UPARAM(ref) FVector& InOutVelocity);

	/** If >= 0, overrides the max speed from SharedChaosCharacterMovementSettings. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover|Flying Settings", meta = (ForceUnits = "cm/s"))
	float MaxSpeedOverride = -1.0f;
};
