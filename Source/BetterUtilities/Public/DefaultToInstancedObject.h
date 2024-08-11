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
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void DestroyObject();
	 
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwningActor() const;

protected:
	 
	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyed();
};
