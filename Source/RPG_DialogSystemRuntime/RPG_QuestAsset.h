// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPG_QuestAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_DIALOGSYSTEMRUNTIME_API URPG_QuestAsset : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString QuestName = TEXT("Enter quest name here");
};
