// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoverFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UMoverFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void SetTransforms_WorldSpace(UPARAM(ref) FMoverTickEndData& OutputState, FVector WorldLocation, FRotator WorldOrient, FVector WorldVelocity, UPrimitiveComponent* Base=nullptr, FName BaseBone = NAME_None);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugCapsuleRay(FVector Start, FVector End, float HalfHeight = 80.0f, float Radius = 50.0f, FRotator Rotation = FRotator::ZeroRotator, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugSphereRay(FVector Start, FVector End, float Radius = 50, int32 Segments = 12, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1, uint8 DepthPriority = 0, float Thickness = 1.f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugBoxRay(FVector Start, FVector End, FVector const& Extent, FRotator const& Rotation, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);

	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugLine(FVector Start, FVector End, float DrawTime = 1, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, uint8 DepthPriority = 0, float Thickness = 1.f) ;
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugSphere(FVector const& Center, float Radius = 50, int32 Segments = 12, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1, uint8 DepthPriority = 0, float Thickness = 1.f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugCapsule(FVector const& Center, float HalfHeight = 80.0f, float Radius = 50.0f, FRotator Rotation = FRotator::ZeroRotator, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugBox(FVector const& Center, FVector const& Extent, FRotator const& Rotation, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);
	UFUNCTION(BlueprintCallable)
	static void ChaosDrawDebugPoint(FVector const& Position, FLinearColor DrawColor = FLinearColor::Red, bool bPersistentLines = false, float DrawTime = 1.0f, uint8 DepthPriority = 0, float Thickness = 1.0f);


};
