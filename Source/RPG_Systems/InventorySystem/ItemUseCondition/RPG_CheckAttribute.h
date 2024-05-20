// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "FunctionalTest.h"
#include "RPG_UseItemConditionComponent.h"
#include "UObject/Object.h"
#include "RPG_CheckAttribute.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_CheckAttribute : public URPG_UseItemConditionComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayAttribute TargetAttribute;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EComparisonMethod ComparisonMethod;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (EditConditionHides, EditCondition = "!bUseAttributeToCompare"))
	float Value = 0;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bUseAttributeToCompare = false;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (EditConditionHides, EditCondition = "bUseAttributeToCompare"))
	FGameplayAttribute AttributeToCompare;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (EditConditionHides, EditCondition = "bUseAttributeToCompare"))
	bool bPercentageCompare = false;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (EditConditionHides, EditCondition = "bPercentageCompare"))
	float PercentageValue = 100;
	virtual bool CanUse_Implementation(AActor* Owner) override;
	virtual FText GetCantUseMensage_Implementation() override;
};
