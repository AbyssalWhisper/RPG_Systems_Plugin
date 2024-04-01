// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPG_Interact_Interface.generated.h"

class APlayerController;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPG_Interact_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_SYSTEMS_API IRPG_Interact_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interact(APlayerController* PlayerController);
};
