// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "RPG_QuestAssetFactory.generated.h"

/**
  Permite a criação do objeto de quest no editor
 */

UCLASS()
class RPG_DIALOGSYSTEMEDITOR_API URPG_QuestAssetFactory : public UFactory
{
public:
	GENERATED_BODY()

	URPG_QuestAssetFactory(const FObjectInitializer& ObjectInitializer);
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
		UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
