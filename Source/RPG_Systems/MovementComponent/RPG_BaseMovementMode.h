// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultToInstancedObject.h"
#include "RPG_BaseMovementMode.generated.h"

/**
 * 
 */
class URPG_MovementComponent;
UCLASS(Blueprintable, BlueprintType)
class RPG_SYSTEMS_API URPG_BaseMovementMode : public UDefaultToInstancedObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void PerformMovement(FVector Input, double DeltaTime, URPG_MovementComponent* MovementComponent);
	virtual void PerformMovement_Implementation(FVector Input,double DeltaTime, URPG_MovementComponent* MovementComponent);
	 
};
