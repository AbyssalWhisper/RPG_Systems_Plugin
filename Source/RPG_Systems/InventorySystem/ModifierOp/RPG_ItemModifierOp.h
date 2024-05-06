// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UObject/NoExportTypes.h"
#include "RPG_ItemModifierOp.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EModifierType : uint8 {
	MagnitudeBased = 0 UMETA(DisplayName = "MagnitudeBased"),
	AttributeBased = 1  UMETA(DisplayName = "AttributeBased"),
};



UCLASS(Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew)
class RPG_SYSTEMS_API URPG_ItemModifierOp : public UObject
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute Attribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModifierType ModifierType = EModifierType::MagnitudeBased;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(EditCondition="EModifierType==EModifierType::MagnitudeBased"))
	float Magnitude = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(EditCondition="EModifierType==EModifierType::AttributeBased"))
	FGameplayAttribute TargetAttribute;
};
