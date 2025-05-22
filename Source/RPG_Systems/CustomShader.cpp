// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomShader.h"

#include "Engine/SubsurfaceProfile.h"
#include "MaterialCompiler.h"
#include "Engine/SpecularProfile.h"

#include "SubstrateDefinitions.h"
#include "Materials/SubstrateMaterial.h"


#if WITH_EDITOR

// Optionnaly cast CodeChunk type to non-LWC type. 
// Input can be built of WorldPosition data, which would force the derived data to have LWC type 
// creating issues, as Substrate functions' inputs don't support LWC
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

// The compilation of an expression can sometimes lead to a INDEX_NONE code chunk when editing material graphs 
// or when the node is inside a material function, linked to an input pin of the material function and that input is not plugged in to anything.
// But for normals or tangents, Substrate absolutely need a valid code chunk to de-duplicate when stored in memory. 
// Also, we want all our nodes to have default, as that is needed when creating BSDF, when registering code chunk representing material topology.
static int32 CompileWithDefaultFloat1(class FMaterialCompiler* Compiler, FExpressionInput& Input, float X, const FScalarMaterialInput* RootNodeInput = nullptr)
{
	int32 DefaultCodeChunk = Compiler->Constant(X);
	if (RootNodeInput && RootNodeInput->UseConstant)
	{
		DefaultCodeChunk = Compiler->Constant(RootNodeInput->Constant);
	}
	int32 CodeChunk = Input.GetTracedInput().Expression ? Input.Compile(Compiler) : DefaultCodeChunk;
	CodeChunk = CastToNonLWCType(Compiler, CodeChunk);
	return CodeChunk == INDEX_NONE ? DefaultCodeChunk : CodeChunk;
}
static int32 CompileWithDefaultFloat2(class FMaterialCompiler* Compiler, FExpressionInput& Input, float X, float Y, const FVector2MaterialInput* RootNodeInput = nullptr)
{
	int32 DefaultCodeChunk = Compiler->Constant2(X, Y);
	if (RootNodeInput && RootNodeInput->UseConstant)
	{
		DefaultCodeChunk = Compiler->Constant2(RootNodeInput->Constant.X, RootNodeInput->Constant.Y);
	}
	int32 CodeChunk = Input.GetTracedInput().Expression ? Input.Compile(Compiler) : DefaultCodeChunk;
	CodeChunk = CastToNonLWCType(Compiler, CodeChunk);
	return CodeChunk == INDEX_NONE ? DefaultCodeChunk : CodeChunk;
}
static int32 CompileWithDefaultFloat3(class FMaterialCompiler* Compiler, FExpressionInput& Input, float X, float Y, float Z, const FColorMaterialInput* RootNodeInput = nullptr)
{
	int32 DefaultCodeChunk = Compiler->Constant3(X, Y, Z);
	if (RootNodeInput && RootNodeInput->UseConstant)
	{
		DefaultCodeChunk = Compiler->Constant3(RootNodeInput->Constant.R, RootNodeInput->Constant.G, RootNodeInput->Constant.B);
	}
	int32 CodeChunk = Input.GetTracedInput().Expression ? Input.Compile(Compiler) : DefaultCodeChunk;
	CodeChunk = CastToNonLWCType(Compiler, CodeChunk);
	return CodeChunk == INDEX_NONE ? DefaultCodeChunk : CodeChunk;
}
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


#endif // WITH_EDITOR




#define LOCTEXT_NAMESPACE "CustomMaterialExpressionSubstrate"
UCustomShader::UCustomShader(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bUseSSSDiffusion(true)
	, SubSurfaceType(EMaterialSubSurfaceType::MSS_Diffusion)
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

	// Sanity check
	static_assert(MSS_MAX == SSS_TYPE_COUNT);
}

#if WITH_EDITOR


int32 UCustomShader::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	/*FGuid PathUniqueId = Compiler->SubstrateTreeStackGetPathUniqueId();
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
	int32 GlintValueCodeChunk = CompileWithDefaultFloat1(Compiler, GlintValue, 1.0f);
	int32 GlintUVCodeChunk = CompileWithDefaultFloat2(Compiler, GlintUV, 0.0f, 0.0f);

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
		GlintValueCodeChunk = Compiler->Constant(1.0f);
	}
	if (!SubstrateOperator.Has(ESubstrateBsdfFeature_SpecularProfile))
	{
		SpecularProfileCodeChunk = INDEX_NONE;
	}
	//Compiler->CustomExpression(TEXT("SUBSTRATE_OPERATOR"),0,0);
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
		Compiler->Constant(SubSurfaceType),
		CompileWithDefaultFloat3(Compiler, EmissiveColor, 0.0f, 0.0f, 0.0f),
		SecondRoughnessCodeChunk,
		SecondRoughnessWeightCodeChunk,
		Compiler->Constant(0.0f),										// SecondRoughnessAsSimpleClearCoat
		NormalCodeChunk,
		FuzzAmountCodeChunk,
		FuzzColorCodeChunk,
		FuzzRoughnessCodeChunk,
		ThicknesCodeChunk,
		GlintValueCodeChunk,
		GlintUVCodeChunk,
		SpecularProfileCodeChunk != INDEX_NONE ? SpecularProfileCodeChunk : Compiler->Constant(0.0f),
		SubstrateOperator.bIsBottom > 0 ? true : false,
		NormalCodeChunk,
		TangentCodeChunk,
		Compiler->GetSubstrateSharedLocalBasisIndexMacro(NewRegisteredSharedLocalBasis),
		!SubstrateOperator.bUseParameterBlending || (SubstrateOperator.bUseParameterBlending && SubstrateOperator.bRootOfParameterBlendingSubTree) ? &SubstrateOperator : nullptr);
	
	return OutputCodeChunk;*/
	return 0;
}



FSubstrateMaterialComplexity UCustomShader::GetHighestComplexity() const
{
	// This function returns the highest complexity of a materials.
	// It will be lowered depending on features enabled per platform.

	uint32 FeatureMask = ESubstrateBsdfFeature_None;
	if (HasGlint()) 			{ FeatureMask |= ESubstrateBsdfFeature_Glint; } 
	if (HasAnisotropy()) 		{ FeatureMask |= ESubstrateBsdfFeature_Anisotropy; } 
	if (HasSpecularProfile()) 	{ FeatureMask |= ESubstrateBsdfFeature_SpecularProfile; } 
	if (HasEdgeColor()) 		{ FeatureMask |= ESubstrateBsdfFeature_EdgeColor; } 
	if (HasFuzz()) 				{ FeatureMask |= ESubstrateBsdfFeature_Fuzz; } 
	if (HasSecondRoughness()) 	{ FeatureMask |= ESubstrateBsdfFeature_SecondRoughnessOrSimpleClearCoat; } 
	if (HasMFPPluggedIn()) 		{ FeatureMask |= ESubstrateBsdfFeature_MFPPluggedIn; } 
	if (HasSSS()) 				{ FeatureMask |= ESubstrateBsdfFeature_SSS; } 

	FSubstrateMaterialComplexity Out;
	Out.Reset();
	if ((FeatureMask & ESubstrateBsdfFeature_ComplexSpecialMask) != 0)
	{
		Out.bIsComplexSpecial = true;
	}
	else if ((FeatureMask & ESubstrateBsdfFeature_ComplexMask) != 0)
	{
		// Nothing
	}
	else if ((FeatureMask & ESubstrateBsdfFeature_SingleMask) != 0)
	{
		Out.bIsSingle = true;
	}
	else
	{
		Out.bIsSimple = true;
	}

	return Out;
}

void UCustomShader::GetCaption(TArray<FString>& OutCaptions) const
{
	// The node complexity is manually maintained to match FSubstrateCompilationContext::SubstrateGenerateDerivedMaterialOperatorData and shaders.
	OutCaptions.Add(TEXT("Custoom Substrate Slab BSDF - ") + FSubstrateMaterialComplexity::ToString(GetHighestComplexity().SubstrateMaterialType()));
}

void UCustomShader::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Substrate Slab BSDF"));
	OutToolTip.Add(TEXT("Complexity = ") + FSubstrateMaterialComplexity::ToString(GetHighestComplexity().SubstrateMaterialType()));
	OutToolTip.Add(TEXT("The complexity represents the cost of the shading path (Lighting, Lumen, SSS) the material will follow:"));
	OutToolTip.Add(TEXT(" - Simple means the Slab only relies on Diffuse, F0 and Roughness. It will follow a fast shading path."));
	OutToolTip.Add(TEXT(" - Single means the Slab uses more features such as F90, Fuzz, Second Roughness, MFP or SSS. It will follow a more expenssive shading path."));
	OutToolTip.Add(TEXT(" - Complex means a Slab uses anisotropic lighting, with any of the previous features."));
	OutToolTip.Add(TEXT(" - Complex Special means the Slab is using more advanced features such as glints or specular LUT. This is the most expenssive shading path."));
}

uint32 UCustomShader::GetOutputType(int32 OutputIndex)
{
	return MCT_Substrate;
}

uint32 UCustomShader::GetInputType(int32 InputIndex)
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

	check(false);
	return MCT_Float1;
}

FName UCustomShader::GetInputName(int32 InputIndex) const
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

	return TEXT("Unknown");
}

void UCustomShader::GetConnectorToolTip(int32 InputIndex, int32 OutputIndex, TArray<FString>& OutToolTip)
{
	if (OutputIndex == 0)
	{
		OutToolTip.Add(TEXT("TT Ouput"));
		return;
	}

	Super::GetConnectorToolTip(InputIndex, INDEX_NONE, OutToolTip);
}

bool UCustomShader::IsResultSubstrateMaterial(int32 OutputIndex)
{
	return true;
}

void UCustomShader::GatherSubstrateMaterialInfo(FSubstrateMaterialInfo& SubstrateMaterialInfo, int32 OutputIndex)
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

FSubstrateOperator* UCustomShader::SubstrateGenerateMaterialTopologyTree(class FMaterialCompiler* Compiler, class UMaterialExpression* Parent, int32 OutputIndex)
{
	FSubstrateOperator& SubstrateOperator = Compiler->SubstrateCompilationRegisterOperator(SUBSTRATE_OPERATOR_BSDF, Compiler->SubstrateTreeStackGetPathUniqueId(), this->MaterialExpressionGuid, Parent, Compiler->SubstrateTreeStackGetParentPathUniqueId());
	SubstrateOperator.BSDFType = SUBSTRATE_BSDF_TYPE_SLAB;

	if (HasEdgeColor())			{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_EdgeColor; }
	if (HasFuzz())				{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_Fuzz; }
	if (HasSecondRoughness())	{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SecondRoughnessOrSimpleClearCoat; }
	if (HasSSS())				{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SSS; }
	if (HasMFPPluggedIn())		{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_MFPPluggedIn; }
	if (HasAnisotropy())		{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_Anisotropy; }
	if (HasGlint())				{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_Glint; }
	if (HasSpecularProfile())	{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SpecularProfile; }
	if (HasSSSProfile())		{ SubstrateOperator.BSDFFeatures |= ESubstrateBsdfFeature_SecondRoughnessOrSimpleClearCoat; } // If a Slab has a subsurface profile, it will have haziness in order to to support the dual-specular lobe from the profile.

	SubstrateOperator.SubSurfaceType = SubSurfaceType;
	SubstrateOperator.ThicknessIndex = Compiler->SubstrateThicknessStackGetThicknessIndex();
	SubstrateOperator.bBSDFWritesEmissive = EmissiveColor.IsConnected();
	return &SubstrateOperator;
}

bool UCustomShader::HasSSS() const
{
	return SubsurfaceProfile != nullptr || SSSMFP.IsConnected();
}

bool UCustomShader::HasSSSProfile() const
{
	return SubsurfaceProfile != nullptr;
}

bool UCustomShader::HasMFPPluggedIn() const
{
	return SSSMFP.IsConnected();
}

bool UCustomShader::HasEdgeColor() const
{
	return F90.IsConnected();
}

bool UCustomShader::HasFuzz() const
{
	return FuzzAmount.IsConnected();
}

bool UCustomShader::HasFuzzRoughness() const
{
	return FuzzRoughness.IsConnected();
}

bool UCustomShader::HasSecondRoughness() const
{
	return SecondRoughnessWeight.IsConnected();
}

bool UCustomShader::HasAnisotropy() const
{
	return Anisotropy.IsConnected();
}

bool UCustomShader::HasGlint() const
{
	// We do not check Substrate::IsGlintEnabled() here. Because we want the glint coverage to affect lower platforms, so the data must flow.
	// The Translator will disable glint and reduce memory footprint if required for a plaform.
	return GlintValue.IsConnected();
}

bool UCustomShader::HasSpecularProfile() const
{
	// Use IsSpecularLUTEnabled to not promote to ComplexSpecial render path if glint rendering is not enabled and avoid registering such possibility at runtime.
	return SpecularProfile != nullptr && Substrate::IsSpecularProfileEnabled();
}

void UCustomShader::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (GraphNode && PropertyChangedEvent.Property != nullptr)
	{
		GraphNode->ReconstructNode();
		GraphNode->Modify();
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif // WITH_EDITOR
