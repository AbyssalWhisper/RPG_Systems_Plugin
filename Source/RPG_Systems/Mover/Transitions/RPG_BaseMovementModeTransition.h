// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementModeTransition.h"

#include <Engine/EngineTypes.h>

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "RPG_BaseMovementModeTransition.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_BaseMovementModeTransition : public UBaseMovementModeTransition
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "CapsuleTraceByChannelWithRotation", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool CapsuleTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, float HalfHeight, const FRotator Orientation, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);
	UFUNCTION(BlueprintCallable, Category = "Collision", meta = (bIgnoreSelf = "true", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime", Keywords = "sweep"))
	static bool SphereTraceSingle(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius,  ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FCharacterDefaultInputs& GetDefaultInputs(const FSimulationTickParams& Params) const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool HasInputTags(const FSimulationTickParams& Params, FGameplayTag InputTag) const;
	UFUNCTION(BlueprintCallable)
	AActor* GetOwner() const;
	

};
