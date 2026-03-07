// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPtr.h"

#include "BetterUtilitiesAutoFolderSettings.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FBetterUtilitiesAutoFolderClassRule
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	// Actor class to match.
	UPROPERTY(EditAnywhere, Config, Category = "Rule")
	TSoftClassPtr<AActor> ActorClass;

	// If true, derived classes also match.
	UPROPERTY(EditAnywhere, Config, Category = "Rule")
	bool bMatchDerived = true;
#endif
};

USTRUCT(BlueprintType)
struct FBetterUtilitiesAutoFolderRuleList
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Config, Category = "Rule")
	TArray<FBetterUtilitiesAutoFolderClassRule> Rules;
#endif
};

UCLASS(Config = Editor, DefaultConfig, meta = (DisplayName = "BetterUtilities - Auto Folder"))
class BETTERUTILITIES_API UBetterUtilitiesAutoFolderSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	// Global toggle for the feature.
	UPROPERTY(EditAnywhere, Config, Category = "Auto Folder")
	bool bEnabled = true;

	// If true, stops after the first matching rule.
	UPROPERTY(EditAnywhere, Config, Category = "Auto Folder")
	bool bStopAfterFirstMatch = true;

	// Map: FolderPath -> class group.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Config, Category = "Auto Folder")
	TMap<FName, FBetterUtilitiesAutoFolderRuleList> FolderRules;
#endif

	virtual FName GetCategoryName() const override
	{
		return TEXT("Plugins");
	}
};
