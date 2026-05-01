// Copyright Epic Games, Inc. All Rights Reserved.

#include "RigUnit_IsObjectValid.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RigUnit_IsObjectValid)

FRigUnit_IsObjectValid_Execute()
{
	bIsValid = IsValid(Object);
}

