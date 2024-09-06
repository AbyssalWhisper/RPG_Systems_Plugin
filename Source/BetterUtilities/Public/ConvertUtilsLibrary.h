// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConvertUtilsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BETTERUTILITIES_API UConvertUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Utility")
	static float BytesToMegabytes(int64 Bytes);

	UFUNCTION(BlueprintCallable, Category="Utility")
	static float BytesToGigabytes(int64 Bytes);

	UFUNCTION(BlueprintCallable, Category="Utility")
	static float ConvertBytesToPercentage(int64 CurrentBytes, int64 TotalBytes);

};
