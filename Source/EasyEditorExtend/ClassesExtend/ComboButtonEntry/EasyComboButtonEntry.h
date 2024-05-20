// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasyComboButtonComponent.h"
#include "UObject/Object.h"
#include "EasyComboButtonEntry.generated.h"

/**
 * 
 */
class UEasyEditorObjectExecuteCode;
UCLASS()
class EASYEDITOREXTEND_API UEasyComboButtonEntry : public UEasyComboButtonComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties))
	UEasyEditorObjectExecuteCode* CustomExecuteCode;
	virtual void Execute(FMenuBuilder& MenuBuilder) override;
};
