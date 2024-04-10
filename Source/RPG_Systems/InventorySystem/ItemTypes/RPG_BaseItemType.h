// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPG_BaseItemType.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew)
class RPG_SYSTEMS_API URPG_BaseItemType : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString ggg;
};
