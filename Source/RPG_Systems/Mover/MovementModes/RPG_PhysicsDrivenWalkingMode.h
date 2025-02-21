// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"
#include "RPG_PhysicsDrivenWalkingMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_PhysicsDrivenWalkingMode : public UPhysicsDrivenWalkingMode
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void BP_SetTargetHeightOverride(float InTargetHeight)
	{
		SetTargetHeightOverride(InTargetHeight);
	}
	UFUNCTION(BlueprintCallable)
	void BP_ClearTargetHeightOverride()
	{
		ClearTargetHeightOverride();
	}
};
