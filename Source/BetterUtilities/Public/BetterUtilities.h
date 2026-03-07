// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#if WITH_EDITOR
#include "Delegates/Delegate.h"
#endif

class FBetterUtilitiesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#if WITH_EDITOR
private:
	void BindEditorDelegates();
	void UnbindEditorDelegates();
	void OnPostEngineInit();
	void OnLevelActorAdded(class AActor* Actor);

	FDelegateHandle PostEngineInitHandle;
	FDelegateHandle LevelActorAddedHandle;
#endif
};
