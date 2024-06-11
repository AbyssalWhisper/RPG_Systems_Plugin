// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementMode.h"
#include "PhysicsMover/PhysicsMoverSimulationTypes.h"
#include "PhysicsBaseMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UPhysicsBaseMovementMode : public UBaseMovementMode , public IPhysicsCharacterMovementModeInterface
{
public:
	virtual void UpdateConstraintSettings(Chaos::FCharacterGroundConstraint& Constraint) const override;

private:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName NextModeName = "";
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

protected:
	virtual void OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;
	virtual void OnGenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
		FProposedMove& OutProposedMove) const override;
	virtual void OnRegistered(const FName ModeName) override;
	virtual void OnUnregistered() override;

public:
	// Maximum torque the character can apply to rotate in air about the vertical axis
	UPROPERTY(EditAnywhere, Category = "Physics Mover", meta = (ClampMin = "0", UIMin = "0", ForceUnits = "NewtonMeters"))
	float TwistTorqueLimit = 0.0f;

	// Maximum torque the character can apply to remain upright
	UPROPERTY(EditAnywhere, Category = "Physics Mover", meta = (ClampMin = "0", UIMin = "0", ForceUnits = "NewtonMeters"))
	float SwingTorqueLimit = 3000.0f;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetDeltaSecondsFromTimeStep(const FMoverTimeStep& TimeStep) const;
 
	TObjectPtr<const class UCommonLegacyMovementSettings> CommonLegacySettings;
};