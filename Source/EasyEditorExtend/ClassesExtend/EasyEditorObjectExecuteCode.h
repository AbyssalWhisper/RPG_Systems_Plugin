// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityObject.h"
#include "EditorUtilityObject.h"
#include "Components/Viewport.h"
#include "Slate/SceneViewport.h"
#include "Widgets/SViewport.h"
#include "EasyEditorObjectExecuteCode.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew)
class EASYEDITOREXTEND_API UEasyEditorObjectExecuteCode : public UEditorUtilityObject
{
	GENERATED_BODY()

	
	public:
	UFUNCTION(BlueprintCallable)
	void NewTab(FName TabName)
	{
		auto aa = FGlobalTabmanager::Get()->TryInvokeTab(TabName);

		
	}
};
