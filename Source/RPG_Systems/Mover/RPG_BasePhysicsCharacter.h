// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define private public
//#include "MoverExamplesCharacter.h"

#include "RPG_BasePhysicsCharacter.generated.h"
#undef private
/**
 * 
 */

namespace RPG_MovementModes
{
	const FName Climb = TEXT("Climb");
	const FName Crouch = TEXT("Crouch");
}


UCLASS()
class RPG_SYSTEMS_API ARPG_BasePhysicsCharacter : public APawn
{
	GENERATED_BODY()
	//void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult) override;

	UPROPERTY()
	bool bWantsCrouch = false;

	UFUNCTION(BlueprintCallable)
	void TryCrouch();
	UFUNCTION(BlueprintCallable)
	void TryUnCrouch();



	UFUNCTION(BlueprintCallable)
	bool IsClimbing() const;
	UFUNCTION(BlueprintCallable)
	bool IsCrouching() const;
	UFUNCTION(BlueprintCallable)
	bool IsWalking() const;
};
