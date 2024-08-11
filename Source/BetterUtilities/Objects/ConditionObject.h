// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultToInstancedObject.h"
#include "ConditionObject.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BETTERUTILITIES_API UConditionObject : public UDefaultToInstancedObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = ( BlueprintAutocast))
	static bool IsAllTrueCondition(TArray<UConditionObject*> Objects,AActor* Instigator);
	UFUNCTION(BlueprintPure, meta = ( BlueprintAutocast))
	static bool HasAnyTrueCondition(TArray<UConditionObject*> Objects,AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool CheckCondition(AActor* Actor);
	virtual bool CheckCondition_Implementation(AActor* Actor);
};
