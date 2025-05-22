// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameInstance.h"
#include "Online/CoreOnline.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Net/Core/Connection/NetEnums.h"

void URPG_GameInstance::Init()
{

	Super::Init();

	GEngine->OnNetworkFailure().AddUObject(this, &URPG_GameInstance::HandleNetworkFailure);

	if (bAutoLoadAndApplySettings)
	{
		FTimerDelegate LoadAndApplySettings;
		GetWorld()->GetTimerManager().SetTimerForNextTick(this,&URPG_GameInstance::LoadAndApplySettings);
	
	}
	

}

void URPG_GameInstance::LoadAndApplySettings()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	UserSettings->LoadSettings(true);
	UserSettings->ApplySettings(false);
	
}

void URPG_GameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
                                             const FString& ErrorString)
{
	OnNetworkFailure(FailureType, ErrorString);
}

void URPG_GameInstance::OnNetworkFailure_Implementation(ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	
}

