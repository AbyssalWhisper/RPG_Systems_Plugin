// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPlatformFilePak.h"

#include "PakLoader.generated.h"

/**
 * 
 */
UCLASS()
class MODS_API UPakLoader : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	FPakPlatformFile* PakPlatformFile = nullptr;
#if WITH_EDITOR
	IPlatformFile *OriginalPlatformFile = nullptr;
#endif
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	bool LoadAllModsFromDirectory(const FString& ModsDirectory, bool bMountContainersEasy);

	FPakPlatformFile* GetPakPlatformFile();
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PakLoader")
	TArray<FString> GetAvailablePakFiles(const FString &ModsDirectory);
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	bool MountPakFileEasy(const FString& PakFilename);
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	bool MountPakFile(const FString& PakFilename, int32 PakOrder, const FString& MountPath);
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	bool UnmountPakFile(const FString& PakFilename);
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	void RegisterMountPoint(const FString& RootPath, const FString& ContentPath);
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	void UnRegisterMountPoint(const FString& RootPath, const FString& ContentPath);
	
	bool GetRootPathAndContentPathForPak(const FPakFile& PakFile, FString& OutRootPath, FString& OutContentPath);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PakLoader")
	int32 GetPakOrderFromPakFilename(const FString& PakFilePath);
	UFUNCTION(BlueprintCallable, Category = "PakLoader")
	void LoadAssetRegistryFile(const FString& AssetRegistryFile);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PakLoader")
	bool DoesFileExist(const FString& Filename);
};
