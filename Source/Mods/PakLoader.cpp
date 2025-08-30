// Fill out your copyright notice in the Description page of Project Settings.


#include "PakLoader.h"

#include "BetterUtilitiesBPLibrary.h"
#include "IPlatformFilePak.h"
#include "AssetRegistry/AssetRegistryState.h"
#include "Serialization/ArrayReader.h"

bool UPakLoader::LoadAllModsFromDirectory(const FString& ModsDirectory, bool bMountContainersEasy)
{
	FString SearchDirectory = ModsDirectory.IsEmpty() ? FPaths::ProjectDir() / TEXT("Mods") : ModsDirectory;

	if (!FPaths::DirectoryExists(SearchDirectory))
	{
		UBetterUtilities::DebugLog(FString::Printf(TEXT("Mods directory does not exist: %s"), *SearchDirectory));
		return false;
	}

	UBetterUtilities::DebugLog(FString::Printf(TEXT("Loading all mods from directory: %s"), *SearchDirectory));

	int32 LoadedCount = 0;

	// Load PAK files
	TArray<FString> PakFiles = GetAvailablePakFiles(SearchDirectory);
	for (const FString& PakFile : PakFiles)
	{
		if (MountPakFileEasy(PakFile))
		{
			LoadedCount++;
			UBetterUtilities::DebugLog(FString::Printf(TEXT("Successfully mounted PAK file: %s"), *PakFile));
		}
		else
		{
			UBetterUtilities::DebugLog(FString::Printf(TEXT("Failed to mount PAK file: %s"), *PakFile));
		}
	}
	
	return LoadedCount > 0;
}

FPakPlatformFile* UPakLoader::GetPakPlatformFile()
{
	if (!PakPlatformFile)
	{
		/*
			Packaged shipping builds will have a PakFile platform.
			For other build types a new pak platform file will be created.
		*/
		IPlatformFile *CurrentPlatformFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
		if (CurrentPlatformFile)
		{
			UBetterUtilities::DebugLog(TEXT("Found PakPlatformFile"));
			PakPlatformFile = static_cast<FPakPlatformFile*>(CurrentPlatformFile);
		}
		else
		{
			PakPlatformFile = new FPakPlatformFile();

			ensure(PakPlatformFile != nullptr);

			IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

#if WITH_EDITOR
			// Keep the original platform file for non packaged builds.
			OriginalPlatformFile = &PlatformFile;
#endif

			if (PakPlatformFile->Initialize(&PlatformFile, TEXT("")))
			{
				FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
			}
			else
			{
				UBetterUtilities::DebugLog(TEXT("Failed to initialize PakPlatformFile"));
			}
		}
	}

	ensure(PakPlatformFile != nullptr);
	return PakPlatformFile;
}

TArray<FString> UPakLoader::GetAvailablePakFiles(const FString &SearchDirectory)
{
	TArray<FString> AvailablePakFiles;
	
	
	if (!FPaths::DirectoryExists(SearchDirectory))
	{
		UBetterUtilities::DebugLog(FString::Printf(TEXT("Mods directory does not exist: %s"), *SearchDirectory));
		return AvailablePakFiles;
	}

	IFileManager::Get().FindFilesRecursive(AvailablePakFiles, *SearchDirectory, TEXT("*.pak"), true, false);

	UBetterUtilities::DebugLog(FString::Printf(TEXT("Found %d PAK files in directory: %s"), AvailablePakFiles.Num(), *SearchDirectory),EEasylog::Log,true);
	return AvailablePakFiles;
}

bool UPakLoader::MountPakFileEasy(const FString& PakFilePath,FString CustomID)
{
	TRefCountPtr<FPakFile> PakFile = new FPakFile(GetPakPlatformFile(), *PakFilePath, false);

	if (!PakFile->IsValid())
	{
		PakFile.SafeRelease();
		UBetterUtilities::DebugLog(FString::Printf(TEXT("Pak file not valid: %s"), *PakFilePath));
		return false;
	}

	FString RootPath, ContentPath;
	if (!GetRootPathAndContentPathForPak(*PakFile, RootPath, ContentPath))
	{
		PakFile.SafeRelease();
		
		UBetterUtilities::DebugLog(FString::Printf(TEXT("Unable to automatically detect root and content "
			"path for pak file %s. This happens when there is no AssetRegistry.bin in the pak file. Make sure that your pak "
			"file has one or use MountPakFile and RegisterMountPoint to specify them yourself."), *PakFilePath));

		return false;
	}

	FString EmptyMP;
	if (!MountPakFile(PakFilePath, INDEX_NONE, EmptyMP))
	{
		PakFile.SafeRelease();

		UBetterUtilities::DebugLog(FString::Printf(TEXT("Mounting of pak file failed %s"), *PakFilePath));
		return false;
	}

	RegisterMountPoint(RootPath, ContentPath);

	TArray<FString> Files;
	PakFile->FindPrunedFilesAtPath(*PakFile->GetMountPoint(), Files, true, false, true);

	FString AssetRegistryFile;
	for (const FString& File : Files)
	{
		if (File.EndsWith("/AssetRegistry.bin", ESearchCase::CaseSensitive))
		{
			AssetRegistryFile = File;
			break;
		}
	}

	LoadAssetRegistryFile(AssetRegistryFile);
	
	bool bArchive = false;
	GConfig->GetBool(TEXT("/Script/UnrealEd.ProjectPackagingSettings"), TEXT("bShareMaterialShaderCode"), bArchive, GGameIni);

	// extract file name without extension
	FString ContentPathWithoutPak = FPaths::GetBaseFilename(PakFilePath);
	static const FName AssetRegistryModuleName(TEXT("AssetRegistry"));
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryModuleName);
	//AssetRegistryModule.Get().ScanPathsSynchronous({ "/"+ContentPathWithoutPak }, true);  // ou outro path se for diferente

	// debug log scanned paths
	//UBetterUtilities::DebugLog(FString::Printf(TEXT("Scanned path for AssetRegistry: %s"), *ContentPathWithoutPak));
	// dump asset registry
	// Acessar AssetRegistry
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	
	// Argumentos: você escolhe o que incluir
	TArray<FString> Arguments = { TEXT("ObjectPath"), TEXT("Class"), TEXT("PackageName"), TEXT("Tag") };

	// Resultado paginado
	TArray<FString> OutPages;

	// Dumpa com 100 linhas por "página"
	//AssetRegistry.DumpState(Arguments, OutPages, 100);

	// Loga cada "página"
	for (int32 PageIndex = 0; PageIndex < OutPages.Num(); ++PageIndex)
	{
		UE_LOG(LogTemp, Log, TEXT("=== AssetRegistry Dump - Página %d ===\n%s"), PageIndex + 1, *OutPages[PageIndex]);
	}
	
	bool bEnable = !FPlatformProperties::IsServerOnly() && FApp::CanEverRender() && bArchive;

	if (bEnable)
	{
		FShaderCodeLibrary::OpenLibrary(FApp::GetProjectName(), ContentPath);
	}

	if (CustomID.IsEmpty()) CustomID = FPaths::GetBaseFilename(PakFilePath);
	MountedPaks.Add(CustomID, FPakContantInfo{FPaths::GetBaseFilename(PakFilePath),PakFilePath,ContentPath,RootPath});
	
	return true;
}

bool UPakLoader::MountPakFile(const FString &PakFilename, int32 PakOrder, const FString &MountPath)
{
	if (PakOrder == INDEX_NONE)
	{
		PakOrder = GetPakOrderFromPakFilename(PakFilename);
	}

	bool bResult = false;
	if (MountPath.Len() > 0)
	{
		bResult = GetPakPlatformFile()->Mount(*PakFilename, PakOrder, *MountPath);
	}
	else
	{
		// NULL will make the mount to use the pak's mount point
		bResult = GetPakPlatformFile()->Mount(*PakFilename, PakOrder, NULL);
	}
	return bResult;
}

bool UPakLoader::UnmountPakFile(const FString &PakFilename)
{
	return GetPakPlatformFile()->Unmount(*PakFilename);
}

void UPakLoader::RegisterMountPoint(const FString& RootPath, const FString& ContentPath)
{
	FPackageName::RegisterMountPoint(RootPath, ContentPath);
}

void UPakLoader::UnRegisterMountPoint(const FString& RootPath, const FString& ContentPath)
{
	FPackageName::UnRegisterMountPoint(RootPath, ContentPath);
}

bool UPakLoader::GetRootPathAndContentPathForPak(const FPakFile& PakFile, FString& OutRootPath, FString& OutContentPath)
{
	if (!PakFile.IsValid())
		return false;

	TArray<FString> Files;
	PakFile.FindPrunedFilesAtPath(*PakFile.GetMountPoint(), Files, true, false, true);

	FString AssetRegistryFile;
	for (const FString& File : Files)
	{
		if (File.EndsWith("/AssetRegistry.bin", ESearchCase::CaseSensitive))
		{
			AssetRegistryFile = File;
			break;
		}
	}

	if (AssetRegistryFile.Len() < 1)
		return false;

	if (!AssetRegistryFile.RemoveFromEnd("/AssetRegistry.bin"))
		return false;

	int32 Idx;
	if (!AssetRegistryFile.FindLastChar('/', Idx))
		return false;

	FString PluginName = AssetRegistryFile.RightChop(Idx + 1);

	OutRootPath = FString::Printf(TEXT("/%s/"), *PluginName);
	OutContentPath.Empty();
	OutContentPath.Append(AssetRegistryFile);
	OutContentPath.Append("/Content/");

	return true;
}

int32 UPakLoader::GetPakOrderFromPakFilename(const FString& PakFilePath)
{
	if (PakFilePath.StartsWith(FString::Printf(TEXT("%sPaks/%s-"), *FPaths::ProjectContentDir(), FApp::GetProjectName())))
	{
		return 4;
	}
	else if (PakFilePath.StartsWith(FPaths::ProjectContentDir()))
	{
		return 3;
	}
	else if (PakFilePath.StartsWith(FPaths::EngineContentDir()))
	{
		return 2;
	}
	else if (PakFilePath.StartsWith(FPaths::ProjectSavedDir()))
	{
		return 1;
	}

	return 0;
}

void UPakLoader::LoadAssetRegistryFile(const FString &AssetRegistryFile)
{
	if (DoesFileExist(AssetRegistryFile))
	{
		FArrayReader SerializedAssetData;

		if (FFileHelper::LoadFileToArray(SerializedAssetData, *AssetRegistryFile))
		{
			SerializedAssetData.Seek(0);

			static const FName AssetRegistryModuleName(TEXT("AssetRegistry"));
			FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryModuleName);

			FAssetRegistryState PakState;
			PakState.Load(SerializedAssetData);
			AssetRegistryModule.Get().AppendState(PakState);
			UBetterUtilities::DebugLog(FString::Printf(TEXT("Loaded AssetRegistry from %s"), *AssetRegistryFile));
			
		}
	}
}

bool UPakLoader::DoesFileExist(const FString &Filename)
{
	return GetPakPlatformFile()->FileExists(*Filename);
}

FPakContantInfo UPakLoader::GetMountedPak(FString PakName)
{
	if (MountedPaks.Contains(PakName))
	{
		return MountedPaks[PakName];
	}
	return FPakContantInfo();
}

TMap<FString, FPakContantInfo> UPakLoader::GetMountedPaks()
{
	return MountedPaks;
}

void UPakLoader::RemoveMountedPakArray(FString Id)
{
	MountedPaks.Remove(Id);
}
