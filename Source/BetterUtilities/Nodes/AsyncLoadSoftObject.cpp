// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterUtilities/Nodes/AsyncLoadSoftObject.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"


// Defina o LOCTEXT_NAMESPACE no início do arquivo
#define LOCTEXT_NAMESPACE "AsyncLoadSoftObjectNamespace"


FText UAsyncLoadSoftObject::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_AsyncLoadSoftObject", "Async Load SoftObject");
}

FText UAsyncLoadSoftObject::GetMenuCategory() const
{
	return LOCTEXT("K2Node_AsyncLoadSoftObject_Category", "Async");
}

FText UAsyncLoadSoftObject::GetTooltipText() const
{
	return LOCTEXT("K2Node_AsyncLoadSoftObject_TooltipText", "Load class");
}

void UAsyncLoadSoftObject::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	UClass* Action = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(Action))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}

void UAsyncLoadSoftObject::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
}

void UAsyncLoadSoftObject::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

}
