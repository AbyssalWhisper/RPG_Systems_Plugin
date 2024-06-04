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

};
