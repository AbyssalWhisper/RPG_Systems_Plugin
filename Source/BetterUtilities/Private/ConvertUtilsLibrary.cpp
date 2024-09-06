// Fill out your copyright notice in the Description page of Project Settings.


#include "ConvertUtilsLibrary.h"
#include "BetterUtilitiesBPLibrary.h"
float UConvertUtilsLibrary::BytesToMegabytes(int64 Bytes)
{
	return static_cast<float>(Bytes) / (1024.0f * 1024.0f);
}

float UConvertUtilsLibrary::BytesToGigabytes(int64 Bytes)
{
	return static_cast<float>(Bytes) / (1024.0f * 1024.0f * 1024.0f);
}

float UConvertUtilsLibrary::ConvertBytesToPercentage(int64 CurrentBytes, int64 TotalBytes)
{
	return UBetterUtilities::GetPercentFromBytes(CurrentBytes,TotalBytes);
}
