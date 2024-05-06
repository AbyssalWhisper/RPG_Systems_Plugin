// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicButtonEditorExtend.h"

#include "EasyEditorObjectExecuteCode.h"

void UDynamicButtonEditorExtend::Execute()
{
	if (CustomExecuteCode)
	{
		CustomExecuteCode->Run();
	}
}
