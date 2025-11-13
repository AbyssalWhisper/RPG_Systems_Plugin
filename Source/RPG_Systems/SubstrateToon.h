// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpressionSubstrate.h"
#include "SubstrateToon.generated.h"

/**
 * 
 */
UCLASS(collapsecategories, hidecategories = Object)
class RPG_SYSTEMS_API USubstrateToon : public UMaterialExpressionSubstrateBSDF
{
	GENERATED_BODY()

public:
	USubstrateToon(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual int32 CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
#endif

	//~ End UMaterialExpression Interface
};
