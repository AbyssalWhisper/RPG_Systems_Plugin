// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPG_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_GameInstance : public UGameInstance
{
private:
	GENERATED_BODY()
	
public:
	UPROPERTY(blueprintReadWrite, EditAnywhere)
	bool bAutoLoadAndApplySettings = true;

	
	virtual void Init() override;
	

	void LoadAndApplySettings();

	virtual void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	UFUNCTION(BlueprintNativeEvent, Category = "Network")
	void OnNetworkFailure(ENetworkFailure::Type FailureType, const FString& ErrorString);
};
