// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPG_ItemDropSound.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_ItemDropSound : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:UPROPERTY(BlueprintReadWrite, EditAnywhere,meta = (EditConditionHides, EditCondition = "bCanDrop"))
	TMap<TEnumAsByte<EPhysicalSurface>,TSoftObjectPtr<USoundBase>> DropSound;
};
