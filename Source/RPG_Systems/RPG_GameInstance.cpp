// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameInstance.h"

void URPG_GameInstance::Init()
{

	Super::Init();

	GEngine->OnNetworkFailure().AddUObject(this, &URPG_GameInstance::HandleNetworkFailure);
}

void URPG_GameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	OnNetworkFailure(FailureType, ErrorString);
}

void URPG_GameInstance::OnNetworkFailure_Implementation(ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	
}

