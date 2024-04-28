// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPG_PlayerInterface.generated.h"

class AMasterItem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPG_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_SYSTEMS_API IRPG_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Open_Container(URPG_InventoryComponent* Container);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Try_PickupItem(ARPG_MasterItem* Item);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class URPG_InteractComponent* GetInteractComponent();
};
