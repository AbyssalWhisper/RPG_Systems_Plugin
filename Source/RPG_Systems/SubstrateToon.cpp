// Fill out your copyright notice in the Description page of Project Settings.


#include "SubstrateToon.h"

#include "MaterialCompiler.h"

USubstrateToon::USubstrateToon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
int32 USubstrateToon::CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	// Compile the SubstrateData output.
	int32 SubstrateDataCodeChunk = Compile(Compiler, OutputIndex);
	// Convert the SubstrateData to a preview color.
	int32 PreviewCodeChunk = Compiler->SubstrateCompilePreview(SubstrateDataCodeChunk);
	return PreviewCodeChunk;
}
#endif
