// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"
#include "AsyncLoadSoftObject.generated.h"

/**
 * 
 */
UCLASS()
class BETTERUTILITIES_API UAsyncLoadSoftObject : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()
	
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	FText GetMenuCategory() const override;

	FText GetTooltipText() const override;

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	//Allocate default pins for a given node
	virtual void AllocateDefaultPins() override;

	//Expands a node while compiling, which may add additional nodes or delete this node.
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
};
