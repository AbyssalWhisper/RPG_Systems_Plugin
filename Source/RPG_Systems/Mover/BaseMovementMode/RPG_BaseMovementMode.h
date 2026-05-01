// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosSimpleFlyingMode.h"

#include "PhysicsMover/PhysicsMoverSimulationTypes.h"

#include "MovementMode.h"
#include "ChaosMover/ChaosMovementMode.h" 
#include "ChaosMover/Character/Modes/ChaosFlyingMode.h"
#include "ChaosMover/Character/Modes/ChaosWalkingMode.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"

#include "RPG_BaseMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_BaseMovementMode : public UChaosSimpleFlyingMode
{
public:
	//virtual void UpdateConstraintSettings(Chaos::FCharacterGroundConstraint& Constraint) const override;

private:
	GENERATED_BODY()

public:
	URPG_BaseMovementMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName NextModeName = "";


#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

public:
	
 
	virtual void SimulationTick_Implementation(const FSimulationTickParams& Params, FMoverTickEndData& OutputState) override;
	virtual void GenerateMove_Implementation(const FMoverSimContext& SimContext, const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
		FProposedMove& OutProposedMove) const override;
 
 
	virtual void OnRegistered(const FName ModeName, const FMoverSimContext& SimContext) override;
	virtual void OnUnregistered(const FMoverSimContext& SimContext) override;
	

public:
	 
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetDeltaSecondsFromTimeStep(const FMoverTimeStep& TimeStep) const;
UFUNCTION(BlueprintCallable)
	void SwitchToState(const FName& StateName, const FSimulationTickParams& Params, FMoverTickEndData& OutputState);
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<const class USharedChaosCharacterMovementSettings> ChaosCharacterMovementSettings;

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
 
	virtual void UpdateConstraintSettings(Chaos::FCharacterGroundConstraintSettings& ConstraintSettings) const override;
 
	UFUNCTION( BlueprintCallable, Category = "Mover|Movement")
	FFloorCheckResult GetCurrentFloor() const;
	

};



