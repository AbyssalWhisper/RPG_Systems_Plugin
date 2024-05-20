// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasyComboButtonComponent.h"
#include "UObject/Object.h"
#include "EasyComboButtonSubMenu.generated.h"

/**
 * 
 */
UCLASS()
class EASYEDITOREXTEND_API UEasyComboButtonSubMenu : public UEasyComboButtonComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties,TitleProperty = ButtonLabel))
	TArray<UEasyComboButtonComponent*> SubMenuList;
	void CreateSubMenu(FMenuBuilder& MenuBuilder);
	virtual void Execute(FMenuBuilder& MenuBuilder) override;
};
