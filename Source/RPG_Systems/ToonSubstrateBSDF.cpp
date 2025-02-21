// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonSubstrateBSDF.h"

#include "MaterialCompiler.h"
#include "Engine/SpecularProfile.h"
#include "Engine/SubsurfaceProfile.h"
#include "Runtime/Engine/Private/Materials/SubstrateMaterial.h"

#define LOCTEXT_NAMESPACE "ToonExpressionSubstrate"

FSubstrateRegisteredSharedLocalBasis SubstrateCompilationInfoCreateSharedLocalBasis(FMaterialCompiler* Compiler, int32 NormalCodeChunk, int32 TangentCodeChunk)
{
	if (TangentCodeChunk == INDEX_NONE)
	{
		return Compiler->SubstrateCompilationInfoRegisterSharedLocalBasis(NormalCodeChunk);
	}
	return Compiler->SubstrateCompilationInfoRegisterSharedLocalBasis(NormalCodeChunk, TangentCodeChunk);
}

#if WITH_EDITOR
int32 UMaterialExpressionSubstrateBSDF::CompilePreview(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	// Compile the SubstrateData output.
	int32 SubstrateDataCodeChunk = Compile(Compiler, OutputIndex);
	// Convert the SubstrateData to a preview color.
	int32 PreviewCodeChunk = Compiler->SubstrateCompilePreview(SubstrateDataCodeChunk);
	return PreviewCodeChunk;
}
#endif


static int32 CompileWithDefaultNormalWS(class FMaterialCompiler* Compiler, FExpressionInput& Input, bool bConvertToRequestedSpace = true)
{
	if (Input.GetTracedInput().Expression != nullptr)
	{
		int32 NormalCodeChunk = Input.Compile(Compiler);

		if (NormalCodeChunk == INDEX_NONE)
		{
			// Nothing is plug in from the linked input, so specify world space normal the BSDF node expects.
			return Compiler->VertexNormal();
		}

		// Ensure the normal has always a valid float3 type
		NormalCodeChunk = Compiler->ForceCast(NormalCodeChunk, MCT_Float3, MFCF_ExactMatch | MFCF_ReplicateValue);

		// Transform into world space normal if needed. BSDF nodes always expects world space normal as input.
		return bConvertToRequestedSpace ? Compiler->TransformNormalFromRequestedBasisToWorld(NormalCodeChunk) : NormalCodeChunk;
	}
	// Nothing is plug in on the BSDF node, so specify world space normal the node expects.
	return Compiler->VertexNormal();
}
static int32 CompileWithDefaultTangentWS(class FMaterialCompiler* Compiler, FExpressionInput& Input, bool bConvertToRequestedSpace = true)
{
	if (Input.GetTracedInput().Expression != nullptr)
	{
		int32 TangentCodeChunk = Input.Compile(Compiler);

		if (TangentCodeChunk == INDEX_NONE)
		{
			// Nothing is plug in from the linked input, so specify world space tangent the BSDF node expects.
			return Compiler->VertexTangent();
		}

		// Ensure the tangent has always a valid float3 type
		TangentCodeChunk = Compiler->ForceCast(TangentCodeChunk, MCT_Float3, MFCF_ExactMatch | MFCF_ReplicateValue);

		// Transform into world space tangent if needed. BSDF nodes always expects world space tangent as input.
		return bConvertToRequestedSpace ? Compiler->TransformNormalFromRequestedBasisToWorld(TangentCodeChunk) : TangentCodeChunk;
	}
	// Nothing is plug in on the BSDF node, so specify world space tangent the node expects.
	return Compiler->VertexTangent();
}

static int32 CreateSubsurfaceProfileParameter(class FMaterialCompiler* Compiler, USubsurfaceProfile* InProfile)
{
	check(InProfile);
	const FName SubsurfaceProfileParameterName = SubsurfaceProfile::CreateSubsurfaceProfileParameterName(InProfile);
	const int32 SSSProfileCodeChunk = Compiler->ForceCast(Compiler->ScalarParameter(SubsurfaceProfileParameterName, 1.0f), MCT_Float1);
	return SSSProfileCodeChunk;
}

static int32 CreateDefaultSubsurfaceProfileParameter(class FMaterialCompiler* Compiler)
{
	const int32 SSSProfileCodeChunk = Compiler->ForceCast(Compiler->ScalarParameter(SubsurfaceProfile::GetSubsurfaceProfileParameterName(), 1.0f), MCT_Float1);
	return SSSProfileCodeChunk;
}


static int32 CastToNonLWCType(class FMaterialCompiler* Compiler, int32 CodeChunk)
{
	EMaterialValueType Type = Compiler->GetType(CodeChunk);
	if (IsLWCType(Type))
	{
		Type = MakeNonLWCType(Type);
		CodeChunk = Compiler->ValidCast(CodeChunk, Type);
	}
	return CodeChunk;
}
static int32 CompileWithDefaultFloat1(class FMaterialCompiler* Compiler, FExpressionInput& Input, float X, bool* bDefaultIsUsed = nullptr)
{
	if (bDefaultIsUsed)
	{
		*bDefaultIsUsed = Input.GetTracedInput().Expression == nullptr;
	}
	int32 CodeChunk = Input.GetTracedInput().Expression ? Input.Compile(Compiler) : Compiler->Constant(X);
	if (bDefaultIsUsed)
	{
		*bDefaultIsUsed |= CodeChunk == INDEX_NONE;
	}
	else
	{
		CodeChunk = CastToNonLWCType(Compiler, CodeChunk);
	}
	return CodeChunk == INDEX_NONE ? Compiler->Constant(X) : CodeChunk;
}
static int32 CompileWithDefaultFloat2(class FMaterialCompiler* Compiler, FExpressionInput& Input, float X, float Y, bool* bDefaultIsUsed = nullptr)
{
	if (bDefaultIsUsed)
	{
		*bDefaultIsUsed = Input.GetTracedInput().Expression == nullptr;
	}
	int32 CodeChunk = Input.GetTracedInput().Expression ? Input.Compile(Compiler) : Compiler->Constant2(X, Y);
	if (bDefaultIsUsed)
	{
		*bDefaultIsUsed |= CodeChunk == INDEX_NONE;
	}
	else
	{
		CodeChunk = CastToNonLWCType(Compiler, CodeChunk);
	}
	return CodeChunk == INDEX_NONE ? Compiler->Constant2(X, Y) : CodeChunk;
}
static int32 CompileWithDefaultFloat3(class FMaterialCompiler* Compiler, FExpressionInput& Input, float X, float Y, float Z, bool* bDefaultIsUsed = nullptr)
{
	if (bDefaultIsUsed)
	{
		*bDefaultIsUsed = Input.GetTracedInput().Expression == nullptr;
	}
	int32 CodeChunk = Input.GetTracedInput().Expression ? Input.Compile(Compiler) : Compiler->Constant3(X, Y, Z);
	if (bDefaultIsUsed)
	{
		*bDefaultIsUsed |= CodeChunk == INDEX_NONE;
	}
	else
	{
		CodeChunk = CastToNonLWCType(Compiler, CodeChunk);
	}
	return CodeChunk == INDEX_NONE ? Compiler->Constant3(X, Y, Z) : CodeChunk;
}

UToonSubstrateBSDF::UToonSubstrateBSDF(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bUseSSSDiffusion(true)
{
	struct FConstructorStatics
	{
		FText NAME_Substrate;
		FConstructorStatics() : NAME_Substrate(LOCTEXT("Substrate BSDFs", "Substrate BSDFs")) { }
	};
	static FConstructorStatics ConstructorStatics;
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_Substrate);
#endif
}

float DielectricSpecularToF0(float SpecularIn)
{
	return 0.08f * SpecularIn;
};
#if WITH_EDITOR

int32 UToonSubstrateBSDF::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	FGuid PathUniqueId = Compiler->SubstrateTreeStackGetPathUniqueId();
	FSubstrateOperator& SubstrateOperator = Compiler->SubstrateCompilationGetOperator(PathUniqueId);

	// We also cannot ignore the tangent when using the default Tangent because GetTangentBasis
	// used in SubstrateGetBSDFSharedBasis cannot be relied on for smooth tangent used for lighting on any mesh.
	const bool bHasAnisotropy = SubstrateOperator.Has(ESubstrateBsdfFeature_Anisotropy);

	int32 SSSProfileCodeChunk = INDEX_NONE;
	if (SubstrateOperator.Has(ESubstrateBsdfFeature_SSS) && SubsurfaceProfile)
	{
		SSSProfileCodeChunk = CreateSubsurfaceProfileParameter(Compiler, SubsurfaceProfile);
	}
	else
	{
		SSSProfileCodeChunk = CreateDefaultSubsurfaceProfileParameter(Compiler);
	}

	int32 SpecularProfileCodeChunk = INDEX_NONE;
	if (SubstrateOperator.Has(ESubstrateBsdfFeature_SpecularProfile))
	{
		const FName SpecularProfileParameterName = SpecularProfile::CreateSpecularProfileParameterName(SpecularProfile);
		SpecularProfileCodeChunk = Compiler->ForceCast(Compiler->ScalarParameter(SpecularProfileParameterName, 1.0f), MCT_Float1);
	}

	const float DefaultSpecular = 0.5f;
	const float DefaultF0 = DielectricSpecularToF0(DefaultSpecular);

	int32 NormalCodeChunk = CompileWithDefaultNormalWS(Compiler, Normal);
	int32 TangentCodeChunk = bHasAnisotropy ? CompileWithDefaultTangentWS(Compiler, Tangent) : INDEX_NONE;
	const FSubstrateRegisteredSharedLocalBasis NewRegisteredSharedLocalBasis = SubstrateCompilationInfoCreateSharedLocalBasis(Compiler, NormalCodeChunk, TangentCodeChunk);

	SubstrateOperator.BSDFRegisteredSharedLocalBasis = NewRegisteredSharedLocalBasis;

	int32 ThicknesCodeChunk = INDEX_NONE;
	if (SubstrateOperator.ThicknessIndex != INDEX_NONE)
	{
		ThicknesCodeChunk = Compiler->SubstrateThicknessStackGetThicknessCode(SubstrateOperator.ThicknessIndex);
	}
	else
	{
		// Thickness is not tracked properly, this can happen when opening a material function in editor
		ThicknesCodeChunk = Compiler->Constant(SUBSTRATE_LAYER_DEFAULT_THICKNESS_CM);
	}
	check(ThicknesCodeChunk != INDEX_NONE);
	
	int32 DiffuseAlbedoCodeChunk = CompileWithDefaultFloat3(Compiler, DiffuseAlbedo, 0.18f, 0.18f, 0.18f);
	int32 F0CodeChunk = CompileWithDefaultFloat3(Compiler, F0, DefaultF0, DefaultF0, DefaultF0);
	int32 RoughnessCodeChunk = CompileWithDefaultFloat1(Compiler, Roughness, 0.5f);
	int32 AnisotropyCodeChunk = CompileWithDefaultFloat1(Compiler, Anisotropy, 0.0f);
	int32 F90CodeChunk = CompileWithDefaultFloat3(Compiler, F90, 1.0f, 1.0f, 1.0f);
	int32 SSSMFPCodeChunk = CompileWithDefaultFloat3(Compiler, SSSMFP, 0.0f, 0.0f, 0.0f);
	int32 SSSMFPScaleCodeChunk = CompileWithDefaultFloat1(Compiler, SSSMFPScale, 1.0f);
	int32 SSSPhaseAnisotropyCodeChunk = CompileWithDefaultFloat1(Compiler, SSSPhaseAnisotropy, 0.0f);
	int32 SecondRoughnessCodeChunk = CompileWithDefaultFloat1(Compiler, SecondRoughness, 0.0f);
	int32 SecondRoughnessWeightCodeChunk = CompileWithDefaultFloat1(Compiler, SecondRoughnessWeight, 0.0f);
	int32 FuzzAmountCodeChunk = CompileWithDefaultFloat1(Compiler, FuzzAmount, 0.0f);
	int32 FuzzColorCodeChunk = CompileWithDefaultFloat3(Compiler, FuzzColor, 0.0f, 0.0f, 0.0f);
	int32 FuzzRoughnessCodeChunk = HasFuzzRoughness() ? CompileWithDefaultFloat1(Compiler, FuzzRoughness, 0.5f) : RoughnessCodeChunk;
	int32 GlintValueCodeChunk = CompileWithDefaultFloat1(Compiler, GlintValue, 0.0f);
	int32 GlintUVCodeChunk = CompileWithDefaultFloat2(Compiler, GlintUV, 0.0f, 0.0f);
	int32 Custom0CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom0, 0.1f);
	int32 Custom1CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom1, 0.0f);
	int32 Custom2CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom2, 0.0f);
	int32 Custom3CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom3, 0.0f);
	int32 Custom4CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom4, 0.0f);
	int32 Custom5CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom5, 1.0f);
	int32 Custom6CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom6, 0.0f);
	int32 Custom7CodeChunk				= CompileWithDefaultFloat1(Compiler, Custom7, 0.0f);
	// Disable some features if requested by the simplification process
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_MFPPluggedIn))
	{
		SSSMFPCodeChunk = Compiler->Constant3(0.0f, 0.0f, 0.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_EdgeColor))
	{
		F90CodeChunk = Compiler->Constant3(1.0f, 1.0f, 1.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_Fuzz))
	{
		FuzzAmountCodeChunk = Compiler->Constant(0.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_SecondRoughnessOrSimpleClearCoat))
	{
		SecondRoughnessWeightCodeChunk = Compiler->Constant(0.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_Anisotropy))
	{
		AnisotropyCodeChunk = Compiler->Constant(0.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_Glint))
	{
		GlintValueCodeChunk = Compiler->Constant(0.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_SpecularProfile))
	{
		SpecularProfileCodeChunk = INDEX_NONE;
	}

	int32 OutputCodeChunk = Compiler->SubstrateSlabBSDF(
		DiffuseAlbedoCodeChunk,
		F0CodeChunk,
		F90CodeChunk,
		RoughnessCodeChunk,
		AnisotropyCodeChunk,
		SSSProfileCodeChunk != INDEX_NONE ? SSSProfileCodeChunk : Compiler->Constant(0.0f),
		SSSMFPCodeChunk,
		SSSMFPScaleCodeChunk,
		SSSPhaseAnisotropyCodeChunk,
		bUseSSSDiffusion ? Compiler->Constant(1.0f) : Compiler->Constant(0.0f),
		CompileWithDefaultFloat3(Compiler, EmissiveColor, 0.0f, 0.0f, 0.0f),
		SecondRoughnessCodeChunk,
		SecondRoughnessWeightCodeChunk,
		Compiler->Constant(0.0f),										// SecondRoughnessAsSimpleClearCoat
		FuzzAmountCodeChunk,
		FuzzColorCodeChunk,
		FuzzRoughnessCodeChunk,
		ThicknesCodeChunk,
		GlintValueCodeChunk,
		GlintUVCodeChunk,
		Custom0CodeChunk,
		Custom1CodeChunk,
		Custom2CodeChunk,
		Custom3CodeChunk,
		Custom4CodeChunk,
		Custom5CodeChunk,
		Custom6CodeChunk,
		Custom7CodeChunk,
		SpecularProfileCodeChunk != INDEX_NONE ? SpecularProfileCodeChunk : Compiler->Constant(0.0f),
		SubstrateOperator.bIsBottom > 0 ? true : false,
		NormalCodeChunk,
		TangentCodeChunk,
		Compiler->GetSubstrateSharedLocalBasisIndexMacro(NewRegisteredSharedLocalBasis),
		!SubstrateOperator.bUseParameterBlending || (SubstrateOperator.bUseParameterBlending && SubstrateOperator.bRootOfParameterBlendingSubTree) ? &SubstrateOperator : nullptr);

	return OutputCodeChunk;
}

#if ENABLE_MATERIAL_LAYER_PROTOTYPE
int32 UToonSubstrateBSDF::CompileDefaultSlab(FMaterialCompiler* Compiler, FVector3f DiffuseAlbedoOverride)
{
	/**
	 * This creates a default slab, primarily for material function previews where the input may be optional/missing.
	 */
	if (!Substrate::IsSubstrateEnabled())
	{
		return Compiler->SubstrateCreateAndRegisterNullMaterial();
	}

	FGuid PathUniqueId = Compiler->SubstrateTreeStackGetPathUniqueId();
	FSubstrateOperator& SubstrateOperator = Compiler->SubstrateCompilationGetOperator(PathUniqueId);

	int32 SSSProfileCodeChunk = INDEX_NONE;
	int32 SpecularProfileCodeChunk = INDEX_NONE;
	int32 TangentCodeChunk = INDEX_NONE;
	int32 NormalCodeChunk = Compiler->VertexNormal();
	int32 ThicknessCodeChunk = Compiler->Constant(SUBSTRATE_LAYER_DEFAULT_THICKNESS_CM);

	const FSubstrateRegisteredSharedLocalBasis NewRegisteredSharedLocalBasis = SubstrateCompilationInfoCreateSharedLocalBasis(Compiler, NormalCodeChunk, TangentCodeChunk);
	SubstrateOperator.BSDFRegisteredSharedLocalBasis = NewRegisteredSharedLocalBasis;

	int32 DiffuseAlbedoCodeChunk = Compiler->Constant3(DiffuseAlbedoOverride.X, DiffuseAlbedoOverride.Y, DiffuseAlbedoOverride.Z);
	const float DefaultF0 = 0.04f;
	int32 F0CodeChunk = Compiler->Constant3(DefaultF0, DefaultF0, DefaultF0);
	int32 RoughnessCodeChunk = Compiler->Constant(0.5f);
	int32 AnisotropyCodeChunk = Compiler->Constant(0.0f);
	int32 F90CodeChunk = Compiler->Constant3(1.0f, 1.0f, 1.0f);
	int32 SSSMFPCodeChunk = Compiler->Constant3(0.0f, 0.0f, 0.0f);
	int32 SSSMFPScaleCodeChunk = Compiler->Constant(1.0f);
	int32 SSSPhaseAnisotropyCodeChunk = Compiler->Constant(0.0f);
	int32 SecondRoughnessCodeChunk = Compiler->Constant(0.0f);
	int32 SecondRoughnessWeightCodeChunk = Compiler->Constant(0.0f);
	int32 FuzzAmountCodeChunk = Compiler->Constant(0.0f);
	int32 FuzzColorCodeChunk = Compiler->Constant3(0.0f, 0.0f, 0.0f);
	int32 FuzzRoughnessCodeChunk = Compiler->Constant(0.5f);
	int32 GlintValueCodeChunk = Compiler->Constant(0.0f);
	int32 GlintUVCodeChunk = Compiler->Constant2(0.0f, 0.0f);

	return Compiler->SubstrateSlabBSDF(
		DiffuseAlbedoCodeChunk,
		F0CodeChunk,
		F90CodeChunk,
		RoughnessCodeChunk,
		AnisotropyCodeChunk,
		Compiler->Constant(0.0f),
		SSSMFPCodeChunk,
		SSSMFPScaleCodeChunk,
		SSSPhaseAnisotropyCodeChunk,
		Compiler->Constant(0.0f),
		Compiler->Constant3(0.0f, 0.0f, 0.0f),
		SecondRoughnessCodeChunk,
		SecondRoughnessWeightCodeChunk,
		Compiler->Constant(0.0f),										// SecondRoughnessAsSimpleClearCoat
		FuzzAmountCodeChunk,
		FuzzColorCodeChunk,
		FuzzRoughnessCodeChunk,
		ThicknessCodeChunk,
		GlintValueCodeChunk,
		GlintUVCodeChunk,
		Compiler->Constant(0.0f),
		false,
		NormalCodeChunk,
		TangentCodeChunk,
		Compiler->GetSubstrateSharedLocalBasisIndexMacro(NewRegisteredSharedLocalBasis),
		(SubstrateOperator.Index != INDEX_NONE && SubstrateOperator.BSDFIndex != INDEX_NONE) && (!SubstrateOperator.bUseParameterBlending || (SubstrateOperator.bUseParameterBlending && SubstrateOperator.bRootOfParameterBlendingSubTree)) ? &SubstrateOperator : nullptr);
}
#endif //ENABLE_MATERIAL_LAYER_PROTOTYPE

FSubstrateMaterialComplexity UToonSubstrateBSDF::GetComplexity() const
{
	FSubstrateMaterialComplexity Out;
	Out.bIsComplexSpecial = HasGlint() || HasSpecularProfile();
	Out.bIsSingle = !HasAnisotropy() && (HasEdgeColor() || HasFuzz() || HasSecondRoughness() || HasMFPPluggedIn() || HasSSS());
	Out.bIsSimple = !HasAnisotropy();

	// Mask out to only have a single possibility
	Out.bIsSingle &= !Out.bIsComplexSpecial;
	Out.bIsSimple &= !Out.bIsComplexSpecial && !Out.bIsSingle;

	return Out;
}

void UToonSubstrateBSDF::GetCaption(TArray<FString>& OutCaptions) const
{
	// The node complexity is manually maintained to match FSubstrateCompilationContext::SubstrateGenerateDerivedMaterialOperatorData and shaders.
	OutCaptions.Add(TEXT("Substrate Slab BSDF - ") + FSubstrateMaterialComplexity::ToString(GetComplexity().SubstrateMaterialType()));
}

void UToonSubstrateBSDF::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Substrate Slab BSDF"));
	OutToolTip.Add(TEXT("Complexity = ") + FSubstrateMaterialComplexity::ToString(GetComplexity().SubstrateMaterialType()));
	OutToolTip.Add(TEXT("The complexity represents the cost of the shading path (Lighting, Lumen, SSS) the material will follow:"));
	OutToolTip.Add(TEXT(" - Simple means the Slab only relies on Diffuse, F0 and Roughness. It will follow a fast shading path."));
	OutToolTip.Add(TEXT(" - Single means the Slab uses more features such as F90, Fuzz, Second Roughness, MFP or SSS. It will follow a more expenssive shading path."));
	OutToolTip.Add(TEXT(" - Complex means a Slab uses anisotropic lighting, with any of the previous features."));
	OutToolTip.Add(TEXT(" - Complex Special means the Slab is using more advanced features such as glints or specular LUT. This is the most expenssive shading path."));
}

uint32 UToonSubstrateBSDF::GetOutputType(int32 OutputIndex)
{
	return MCT_Substrate;
}

uint32 UToonSubstrateBSDF::GetInputType(int32 InputIndex)
{
	if (InputIndex == 0)
	{
		return MCT_Float3; // DiffuseAlbedo
	}
	else if (InputIndex == 1)
	{
		return MCT_Float3; // F0
	}
	else if (InputIndex == 2)
	{
		return MCT_Float3; // F90
	}
	else if (InputIndex == 3)
	{
		return MCT_Float1; // Roughness
	}
	else if (InputIndex == 4)
	{
		return MCT_Float1; // Anisotropy
	}
	else if (InputIndex == 5)
	{
		return MCT_Float3; // Normal
	}
	else if (InputIndex == 6)
	{
		return MCT_Float3; // Tangent
	}
	else if (InputIndex == 7)
	{
		return MCT_Float3; // SSSMFP
	}
	else if (InputIndex == 8)
	{
		return MCT_Float1; // SSSMFPScale
	}
	else if (InputIndex == 9)
	{
		return MCT_Float1; // SSSPhaseAniso
	}
	else if (InputIndex == 10)
	{
		return MCT_Float3; // Emissive Color
	}
	else if (InputIndex == 11)
	{
		return MCT_Float1; // SecondRoughness
	}
	else if (InputIndex == 12)
	{
		return MCT_Float1; // SecondRoughnessWeight
	}
	else if (InputIndex == 13)
	{
		return MCT_Float1; // FuzzRoughness
	}
	else if (InputIndex == 14)
	{
		return MCT_Float1; // FuzzAmount
	}
	else if (InputIndex == 15)
	{
		return MCT_Float3; // FuzzColor
	}
	else if (InputIndex == 16)
	{
		return MCT_Float; // GlintValue
	}
	else if (InputIndex == 17)
	{
		return MCT_Float2; // GlintUV
	}
	else if(InputIndex == 18)
	{
		return MCT_Float; //Dylan add Custom0
	}else if(InputIndex == 19)
	{
		return MCT_Float; //Dylan add Custom1
	}else if(InputIndex == 20)
	{
		return MCT_Float; //Dylan add Custom2
	}
	else if(InputIndex == 21)
	{
		return MCT_Float; //Dylan add Custom3
	}else if(InputIndex == 22)
	{
		return MCT_Float; //Dylan add Custom0
	}else if(InputIndex == 23)
	{
		return MCT_Float; //Dylan add Custom1
	}else if(InputIndex == 24)
	{
		return MCT_Float; //Dylan add Custom2
	}
	else if(InputIndex == 25)
	{
		return MCT_Float; //Dylan add Custom3}
	}
	
	check(false);
	return MCT_Float1;
}

FName UToonSubstrateBSDF::GetInputName(int32 InputIndex) const
{
	if (InputIndex == 0)
	{
		return TEXT("Diffuse Albedo");
	}
	else if (InputIndex == 1)
	{
		return TEXT("F0");
	}
	else if (InputIndex == 2)
	{
		return  TEXT("F90");
	}
	else if (InputIndex == 3)
	{
		return TEXT("Roughness");
	}
	else if (InputIndex == 4)
	{
		return TEXT("Anisotropy");
	}
	else if (InputIndex == 5)
	{
		return TEXT("Normal");
	}
	else if (InputIndex == 6)
	{
		return TEXT("Tangent");
	}
	else if (InputIndex == 7)
	{
		return TEXT("SSS MFP");
	}
	else if (InputIndex == 8)
	{
		return TEXT("SSS MFP Scale");
	}
	else if (InputIndex == 9)
	{
		return TEXT("SSS Phase Anisotropy");
	}
	else if (InputIndex == 10)
	{
		return TEXT("Emissive Color");
	}
	else if (InputIndex == 11)
	{
		return TEXT("Second Roughness");
	}
	else if (InputIndex == 12)
	{
		return TEXT("Second Roughness Weight");
	}
	else if (InputIndex == 13)
	{
		return TEXT("Fuzz Roughness");
	}
	else if (InputIndex == 14)
	{
		return TEXT("Fuzz Amount");
	}
	else if (InputIndex == 15)
	{
		return TEXT("Fuzz Color");
	}
	else if (InputIndex == 16)
	{
		return Substrate::IsGlintEnabled() ? TEXT("Glint Density") : TEXT("Glint Density (Disabled)");
	}
	else if (InputIndex == 17)
	{
		return Substrate::IsGlintEnabled() ? TEXT("Glint UVs") : TEXT("Glint UVs (Disabled)");
	}
	else if(InputIndex == 18)
	{
		return TEXT("Toon Shadow Offset"); //Custom 0
	}else if(InputIndex == 19)
	{
		return TEXT("Masked Color R"); // Custom 1
	}else if(InputIndex == 20)
	{
		return TEXT("Masked Color G"); // Custom 2
	}else if(InputIndex == 21)
	{
		return TEXT("Masked Color B"); //Custom 3
	} else if(InputIndex == 22)
	{
		return TEXT("Specular Mask"); //Custom 4
	}else if(InputIndex == 23)
	{
		return TEXT("Custom 5");
	}else if(InputIndex == 24)
	{
		return TEXT("Custom 6");
	}else if(InputIndex == 25)
	{
		return TEXT("Custom 7");
	}
	
	return TEXT("Unknown");
}

void UToonSubstrateBSDF::GetConnectorToolTip(int32 InputIndex, int32 OutputIndex, TArray<FString>& OutToolTip)
{
	if (OutputIndex == 0)
	{
		OutToolTip.Add(TEXT("TT Ouput"));
		return;
	}

	Super::GetConnectorToolTip(InputIndex, INDEX_NONE, OutToolTip);
}

bool UToonSubstrateBSDF::IsResultSubstrateMaterial(int32 OutputIndex)
{
	return true;
}

void UToonSubstrateBSDF::GatherSubstrateMaterialInfo(FSubstrateMaterialInfo& SubstrateMaterialInfo, int32 OutputIndex)
{
	// Track connected inputs
	if (DiffuseAlbedo.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_DiffuseColor); }
	if (F0.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_SpecularColor); }
	if (Roughness.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_Roughness); }
	if (Anisotropy.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_Anisotropy); }
	if (EmissiveColor.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_EmissiveColor); }
	if (Normal.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_Normal); }
	if (Tangent.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_Tangent); }
	if (SSSMFP.IsConnected()) { SubstrateMaterialInfo.AddPropertyConnected(MP_SubsurfaceColor); }

	if (HasSSS())
	{
		// We still do not know if this is going to be a real SSS node because it is only possible for BSDF at the bottom of the stack. Nevertheless, we take the worst case into account.
		if (SubsurfaceProfile)
		{
			SubstrateMaterialInfo.AddShadingModel(SSM_SubsurfaceProfile);
			SubstrateMaterialInfo.AddSubsurfaceProfile(SubsurfaceProfile);
		}
		else
		{
			SubstrateMaterialInfo.AddShadingModel(SSM_SubsurfaceMFP);
		}
	}
	else
	{
		SubstrateMaterialInfo.AddShadingModel(SSM_DefaultLit);
	}

	if (HasSpecularProfile())
	{
		SubstrateMaterialInfo.AddSpecularProfile(SpecularProfile);
	}

	SubstrateMaterialInfo.AddGuid(MaterialExpressionGuid);
}

FSubstrateOperator* UToonSubstrateBSDF::SubstrateGenerateMaterialTopologyTree(class FMaterialCompiler* Compiler, class UMaterialExpression* Parent, int32 OutputIndex)
{
	FSubstrateOperator& SubstrateOperator = Compiler->SubstrateCompilationRegisterOperator(SUBSTRATE_OPERATOR_BSDF, Compiler->SubstrateTreeStackGetPathUniqueId(), this, Parent, Compiler->SubstrateTreeStackGetParentPathUniqueId());
	SubstrateOperator.BSDFType = SUBSTRATE_BSDF_TYPE_SLAB;

	if (HasEdgeColor())			{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_EdgeColor; }
	if (HasFuzz())				{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_Fuzz; }
	if (HasSecondRoughness())	{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SecondRoughnessOrSimpleClearCoat; }
	if (HasSSS())				{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SSS; }
	if (HasMFPPluggedIn())		{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_MFPPluggedIn; }
	if (HasAnisotropy())		{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_Anisotropy; }
	if (HasGlint())				{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_Glint; }
	if (HasSpecularProfile())	{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SpecularProfile; }

	SubstrateOperator.ThicknessIndex = Compiler->SubstrateThicknessStackGetThicknessIndex();
	SubstrateOperator.bBSDFWritesEmissive = EmissiveColor.IsConnected();
	return &SubstrateOperator;
}

bool UToonSubstrateBSDF::HasSSS() const
{
	return SubsurfaceProfile != nullptr || SSSMFP.IsConnected();
}

bool UToonSubstrateBSDF::HasSSSProfile() const
{
	return SubsurfaceProfile != nullptr;
}

bool UToonSubstrateBSDF::HasMFPPluggedIn() const
{
	return SSSMFP.IsConnected();
}

bool UToonSubstrateBSDF::HasEdgeColor() const
{
	return F90.IsConnected();
}

bool UToonSubstrateBSDF::HasFuzz() const
{
	return FuzzAmount.IsConnected();
}

bool UToonSubstrateBSDF::HasFuzzRoughness() const
{
	return FuzzRoughness.IsConnected();
}

bool UToonSubstrateBSDF::HasSecondRoughness() const
{
	return SecondRoughnessWeight.IsConnected();
}

bool UToonSubstrateBSDF::HasAnisotropy() const
{
	return Anisotropy.IsConnected();
}

bool UToonSubstrateBSDF::HasGlint() const
{
	// Use IsGlintEnabled to not promote to ComplexSpecial render path if glint rendering is not enabled and avoid registering such possibility at runtime.
	return GlintValue.IsConnected() && Substrate::IsGlintEnabled();
}

bool UToonSubstrateBSDF::HasSpecularProfile() const
{
	// Use IsSpecularLUTEnabled to not promote to ComplexSpecial render path if glint rendering is not enabled and avoid registering such possibility at runtime.
	return SpecularProfile != nullptr && Substrate::IsSpecularProfileEnabled();
}

#endif // WITH_EDITOR

