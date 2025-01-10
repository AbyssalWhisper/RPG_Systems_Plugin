// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"


/**
 * 
 */
class  RPG_QuestAssetEditorPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	RPG_QuestAssetEditorPrimaryTabFactory(TSharedPtr<class RPG_QuestEditorApp> app);
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class RPG_QuestEditorApp> _app;
};
