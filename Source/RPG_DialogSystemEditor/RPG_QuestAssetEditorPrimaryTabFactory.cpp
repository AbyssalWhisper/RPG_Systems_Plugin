// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_QuestAssetEditorPrimaryTabFactory.h"
#include "RPG_QuestEditorApp.h"

RPG_QuestAssetEditorPrimaryTabFactory::RPG_QuestAssetEditorPrimaryTabFactory(
	TSharedPtr<class RPG_QuestEditorApp> app) :
FWorkflowTabFactory(FName("RPG_QuestAssetPrimaryTab"),app)
{
	_app = app;
	// Define o nome da aba 
	TabLabel = FText::FromString("Primary");
	ViewMenuDescription = FText::FromString("Display a primary view of the asset");
	ViewMenuTooltip = FText::FromString("Show the primary view");
}

TSharedRef<SWidget> RPG_QuestAssetEditorPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(STextBlock).Text(FText::FromString("Primary Tab Body"));
}

FText RPG_QuestAssetEditorPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString("Primary Tab ToolTip");
}
