// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

/**
 * 
 */
class RPG_QuestEditorApp : public FWorkflowCentricApplication , public FEditorUndoClient ,public FNotifyHook
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void InitEditor (const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);

	virtual FText GetBaseToolkitName() const override { return FText::FromString("QuestAssetEditorApp"); }

protected:
	virtual FString GetDocumentationLink() const override { return TEXT(""); }

public:
	virtual FName GetToolkitFName() const override { return FName("QuestAssetEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f,0.2f,0.5f,0.5f);}
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("QuestAssetEditorApp"); }
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
};
