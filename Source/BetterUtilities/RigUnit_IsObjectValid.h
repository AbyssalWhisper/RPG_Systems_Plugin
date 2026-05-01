// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UAF/Internal/AnimNextExecuteContext.h"
#include "UAF/Internal/Graph/RigUnit_AnimNextBase.h"

#include "RigUnit_IsObjectValid.generated.h"

/** Checks whether a given object is valid (non-null and not pending kill) */
USTRUCT(meta=(DisplayName="Is Object Valid", Category="Utilities", NodeColor="0, 0.5, 1", Keywords="Null,Check,Valid,Object"))
struct FRigUnit_IsObjectValid : public FRigUnit_AnimNextBase
{
	GENERATED_BODY()

	RIGVM_METHOD()
	void Execute();

	virtual FString GetUnitSubTitle() const { return TEXT("Is Valid"); };

	// The object to check
	UPROPERTY(EditAnywhere, Category = "Graph", meta = (Input))
	TObjectPtr<UObject> Object;

	// True if the object is valid
	UPROPERTY(EditAnywhere, Category = "Graph", meta = (Output))
	bool bIsValid = false;

	// The execution result
	UPROPERTY(EditAnywhere, DisplayName = "Execute", Category = "BeginExecution", meta = (Input, Output))
	FAnimNextExecuteContext ExecuteContext;
};

