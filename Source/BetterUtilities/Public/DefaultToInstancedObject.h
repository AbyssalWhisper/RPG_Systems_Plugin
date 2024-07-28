// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefaultToInstancedObject.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, CollapseCategories)
class BETTERUTILITIES_API UDefaultToInstancedObject : public UObject
{
	GENERATED_BODY()
public:

	//UObject interface implementation
	virtual UWorld* GetWorld() const override;
	//End of implementation

	//Will mark this UObject as garbage and will eventually get cleaned by the garbage collector.
	//Should only execute this on the server.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Replicated UObject", DisplayName = "Destroy Replicated UObject")
	void DestroyObject();

	//Gets the Actor that "owns" this Replicated UObject.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Replicated UObject")
	AActor* GetOwningActor() const;

protected:

	//Will get called on the server once the Replicated UObject has been marked as garbage.
	UFUNCTION(BlueprintImplementableEvent, Category = "Replicated UObject", DisplayName = "On Replicated UObject Destroyed")
	void OnDestroyed();
};
