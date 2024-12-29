// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PhysicsMover/PhysicsMoverSimulationTypes.h"

#include "MovementMode.h"

#include "RPG_BaseMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_BaseMovementMode : public UBaseMovementMode
{
public:
	//virtual void UpdateConstraintSettings(Chaos::FCharacterGroundConstraint& Constraint) const override;

private:
	GENERATED_BODY()

public:
	URPG_BaseMovementMode();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName NextModeName = "";


#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

protected:
	virtual void OnSimulationTick(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;
	
	void CaptureFinalState(USceneComponent* UpdatedComponent, FMovementRecord& Record, const FMoverDefaultSyncState& StartSyncState, FMoverDefaultSyncState& OutputSyncState, const float DeltaSeconds) const;

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
UFUNCTION(BlueprintCallable)
	void SwitchToState(const FName& StateName, const FSimulationTickParams& Params, FMoverTickEndData& OutputState);
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<const class UCommonLegacyMovementSettings> CommonLegacySettings;

	UFUNCTION(BlueprintCallable, Category = "Mover")
	static FFreeMoveParams MakeFreeMoveParams( 
								  EMoveInputType MoveInputType, 
								  FVector MoveInput, 
								  FRotator OrientationIntent, 
								  FVector PriorVelocity, 
								  FRotator PriorOrientation, 
								  float MaxSpeed, 
								  float Acceleration, 
								  float Deceleration, 
								  float TurningBoost, 
								  float TurningRate, 
								  float DeltaSeconds, 
								  FQuat WorldToGravityQuat, 
								  bool bUseAccelerationForVelocityMove);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FCharacterDefaultInputs& GetDefaultInputs(const FMoverTickStartData& StartState);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FCharacterDefaultInputs& GetDefaultInputsFromInputCmd(const FMoverInputCmdContext& InputCmd);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FMoverDefaultSyncState& GetMoverDefaultSyncState(const FMoverTickStartData& StartState);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FVector GetVelocity_WorldSpace(const FMoverDefaultSyncState& MoverDefaultSyncState);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FRotator GetOrientation_WorldSpace(const FMoverDefaultSyncState& MoverDefaultSyncState);

	UFUNCTION(BlueprintCallable)
	FFloorCheckResult GetCurrentFloor() const;

	UFUNCTION(BlueprintCallable, Category = "Mover|PlanarConstraint")
	FVector ConstrainDirectionToPlane(const FMoverTickStartData& StartState, bool bMaintainMagnitude = false)const;
	
};



