// Copyright Epic Games, Inc. All Rights Reserved.

#include "BetterUtilities.h"

#if WITH_EDITOR
#include "BetterUtilitiesAutoFolderSettings.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/CoreDelegates.h"
#endif

#define LOCTEXT_NAMESPACE "FBetterUtilitiesModule"

void FBetterUtilitiesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	BindEditorDelegates();
#endif
}

void FBetterUtilitiesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

#if WITH_EDITOR
	UnbindEditorDelegates();
#endif
}

#if WITH_EDITOR
void FBetterUtilitiesModule::BindEditorDelegates()
{
	// GEngine might not exist yet depending on loading phase; bind after engine init when needed.
	if (GEngine)
	{
		LevelActorAddedHandle = GEngine->OnLevelActorAdded().AddRaw(this, &FBetterUtilitiesModule::OnLevelActorAdded);
	}
	else
	{
		PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FBetterUtilitiesModule::OnPostEngineInit);
	}
}

void FBetterUtilitiesModule::UnbindEditorDelegates()
{
	if (PostEngineInitHandle.IsValid())
	{
		FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
		PostEngineInitHandle.Reset();
	}

	if (LevelActorAddedHandle.IsValid() && GEngine)
	{
		GEngine->OnLevelActorAdded().Remove(LevelActorAddedHandle);
		LevelActorAddedHandle.Reset();
	}
}

void FBetterUtilitiesModule::OnPostEngineInit()
{
	// Swap from post-init hook to the actual actor-added delegate.
	if (PostEngineInitHandle.IsValid())
	{
		FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
		PostEngineInitHandle.Reset();
	}

	if (GEngine && !LevelActorAddedHandle.IsValid())
	{
		LevelActorAddedHandle = GEngine->OnLevelActorAdded().AddRaw(this, &FBetterUtilitiesModule::OnLevelActorAdded);
	}
}

void FBetterUtilitiesModule::OnLevelActorAdded(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	UWorld* World = Actor->GetWorld();
	if (!World)
	{
		return;
	}

	const UBetterUtilitiesAutoFolderSettings* Settings = GetDefault<UBetterUtilitiesAutoFolderSettings>();
	if (!Settings || !Settings->bEnabled)
	{
		return;
	}

	if (Settings->FolderRules.IsEmpty())
	{
		return;
	}

	// TMap iteration order is not guaranteed; sort keys for stable behavior.
	TArray<FName> FolderKeys;
	Settings->FolderRules.GetKeys(FolderKeys);
	FolderKeys.Sort([](const FName& A, const FName& B) { return A.LexicalLess(B); });

	for (const FName& FolderPath : FolderKeys)
	{
		const FBetterUtilitiesAutoFolderRuleList* RuleList = Settings->FolderRules.Find(FolderPath);
		if (!RuleList || FolderPath.IsNone())
		{
			continue;
		}

		for (const FBetterUtilitiesAutoFolderClassRule& Rule : RuleList->Rules)
		{
			UClass* RuleClass = Rule.ActorClass.Get();
			if (!RuleClass && Rule.ActorClass.ToSoftObjectPath().IsValid())
			{
				RuleClass = Rule.ActorClass.LoadSynchronous();
			}
			if (!RuleClass)
			{
				continue;
			}

			const bool bMatches = Rule.bMatchDerived ? Actor->IsA(RuleClass) : (Actor->GetClass() == RuleClass);
			if (!bMatches)
			{
				continue;
			}

			Actor->SetFolderPath(FolderPath);

			if (Settings->bStopAfterFirstMatch)
			{
				return;
			}

			// If not stopping, don't re-check more rules for this same folder.
			break;
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBetterUtilitiesModule, BetterUtilities)
