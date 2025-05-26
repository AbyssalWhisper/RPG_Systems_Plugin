// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Systems.h"

#define LOCTEXT_NAMESPACE "FRPG_SystemsModule"

void FRPG_SystemsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	//FString ShaderDir = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("RPG_Systems/Shaders"));
	//AddShaderSourceDirectoryMapping("/RPG_Systems", ShaderDir);
}

void FRPG_SystemsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRPG_SystemsModule, RPG_Systems)