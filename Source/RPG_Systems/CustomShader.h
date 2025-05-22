// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpressionSubstrate.h"
#include "CustomShader.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API UCustomShader : public UMaterialExpression
{
public:

private:
	GENERATED_UCLASS_BODY()
	/**
	 * Defines the diffused albedo, the percentage of light reflected as diffuse from the surface. (type = float3, unit = unitless, defaults to 0.18)
	 */
	UPROPERTY()
	FExpressionInput DiffuseAlbedo;

	/**
	 * Defines the color and brightness of the specular highlight where the surface is facing the camera. Each specular contribution will fade to black as F0 drops below 0.02. (type = float3, unit = unitless, defaults to plastic 0.04)
	 */
	UPROPERTY()
	FExpressionInput F0;

	/**
	 * Defines the color of the specular highlight where the surface normal is 90 degrees from the view direction. Only the hue and saturation are preserved, the brightness is fixed at 1.0. Fades to black as F0 drops below 0.02. (type = float3, unit = unitless, defaults to 1.0f)
	 */
	UPROPERTY()
	FExpressionInput F90;

	/**
	 * Controls how rough the Material is. Roughness of 0 (smooth) is a mirror reflection and 1 (rough) is completely matte or diffuse. When using anisotropy, it is the roughness used along the Tangent axis. (type = float, unit = unitless, defaults to 0.5)
	 */
	UPROPERTY()
	FExpressionInput Roughness;
		
	/**
	 * Controls the anisotropy factor of the roughness. Positive value elongates the specular lobe along the Tangent vector, Negative value elongates the specular lobe along the perpendicular of the Tangent. (type = float, unit = unitless).
	 */
	UPROPERTY()
	FExpressionInput Anisotropy;

	/**
	 * Take the surface normal as input. The normal is considered tangent or world space according to the space properties on the main material node. (type = float3, unit = unitless, defaults to vertex normal)
	 */
	UPROPERTY()
	FExpressionInput Normal;

	/**
	* Take a surface tangent as input. The tangent is considered tangent or world space according to the space properties on the main material node. (type = float3, unit = unitless, defaults to vertex tangent)
	*/
	UPROPERTY()
	FExpressionInput Tangent;

	/**
	 * Chromatic mean free path. Only used when there is not any sub-surface profile provided. (type = float3, unit = centimeters, default = 0).
	 * For subsurface scattering, it is recommended to specify the MFP as world space centimeter directly. As it is an intuitive way to specify how far the light will scatter/bleed.
	 * For translucent coat layer, it is recommended to use the TransmittanceToMFP node as it might be easier to specify the transmittance color.
	 */
	UPROPERTY()
	FExpressionInput SSSMFP;

	/**
	 * Scale the mean free path length. Value between 0 and 1. Always used, with or without a subsurface profile. (type = float, unitless, defaults to 1)
	 */
	UPROPERTY()
	FExpressionInput SSSMFPScale;

	/**
	 * Phase function anisotropy. Positive value elongates the phase function along the lignt direction, causing forward scattering. Negative value elongates the phase function backward to the light direction, causing backward scattering.  (type = float, unitless, defaults to 1, valid value -1..1)
	 */
	UPROPERTY()
	FExpressionInput SSSPhaseAnisotropy;

	/**
	 * Emissive color on top of the surface (type = float3, unit = luminance, default = 0)
	 */
	UPROPERTY()
	FExpressionInput EmissiveColor;

	/**
	 * Controls the roughness of a secondary specular lobe. Roughness of 0 (smooth) is a mirror reflection and 1 (rough) is completely matte or diffuse. Does not influence diffuse roughness. (type = float, unit = unitless, defaults to 0.5)
	 */
	UPROPERTY()
	FExpressionInput SecondRoughness;

	/**
	 * The weight of the second specular lobe using SecondRoughness. The first specular using Roughness will have a weight of (1 - SecondRoughnessWeight). (type = float, unitless, default = 0)
	 */
	UPROPERTY()
	FExpressionInput SecondRoughnessWeight;

	/**
	 * Controls how rough the Fuzz layer is. Roughness of 0 is smooth and 1 is rough. If FuzzRoughness Is not connected, the Roughness input will be used instead. (type = float, unit = unitless, defaults to 0.5)
	 */
	UPROPERTY()
	FExpressionInput FuzzRoughness;

	/**
	 * The amount of fuzz on top of the surface used to simulate cloth-like appearance.
	 */
	UPROPERTY()
	FExpressionInput FuzzAmount;

	/**
	 * The base color of the fuzz.
	 */
	UPROPERTY()
	FExpressionInput FuzzColor;

	/**
	 * This represent the micro facet density. Only used when `r.Substrate.Glints=1`. 0 = only very sparse glints, 1 = fully covered with glints (which is equivalent to a regular specular lobe). Defaults to 1 (=no glint).
	 */
	UPROPERTY()
	FExpressionInput GlintValue;

	/**
	 * The parameterization of the surface required to position glints on a surface. Only used when `r.Substrate.Glints=1`. Defaults to (0,0).
	 */
	UPROPERTY()
	FExpressionInput GlintUV;

	/** SubsurfaceProfile, for Screen Space Subsurface Scattering. The profile needs to be set up on both the Substrate diffuse node, and the material node at the moment. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (DisplayName = "Subsurface Profile"))
	TObjectPtr<class USubsurfaceProfile> SubsurfaceProfile;

	/** SpecularProfile, for modulating specular appearance and simulating more complex visuals such as iridescence. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (DisplayName = "Specular Profile"))
	TObjectPtr<class USpecularProfile> SpecularProfile;

	UE_DEPRECATED(5.6, "Please, use SubSurfaceType to enable/disable SSS-diffusion")
	UPROPERTY()
	uint32 bUseSSSDiffusion : 1;

	/** Define sub-subsurface used behavior of the slab. This option trades quality over performance and will result into visual differences. 
	 *   * For slab not sitting at the bottom of the topology (e.g. slabs stacked with a vertial operator), only the SimpleVolume SSS type is available.
	 *   * In non-opaque blend modes, SSS Diffusion and SSS Diffusion Profile are not available and will fallback onto Wrap mode.
	 */
	UPROPERTY(EditAnywhere, Category = Material, meta = (DisplayName = "Sub-Surface Type"))
	TEnumAsByte<enum EMaterialSubSurfaceType> SubSurfaceType = MSS_None;

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual uint32 GetOutputType(int32 OutputIndex) override;
	virtual uint32 GetInputType(int32 InputIndex) override;
	virtual bool IsResultSubstrateMaterial(int32 OutputIndex) override;
	virtual void GatherSubstrateMaterialInfo(FSubstrateMaterialInfo& SubstrateMaterialInfo, int32 OutputIndex) override;
	virtual FSubstrateOperator* SubstrateGenerateMaterialTopologyTree(class FMaterialCompiler* Compiler, class UMaterialExpression* Parent, int32 OutputIndex) override;
	virtual FName GetInputName(int32 InputIndex) const override;
	virtual void GetConnectorToolTip(int32 InputIndex, int32 OutputIndex, TArray<FString>& OutToolTip) override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	bool HasEdgeColor() const;
	bool HasFuzz() const;
	bool HasFuzzRoughness() const;
	bool HasSecondRoughness() const;
	bool HasSSS() const;
	bool HasSSSProfile() const;
	bool HasMFPPluggedIn() const;
	bool HasAnisotropy() const;
	bool HasGlint() const;
	bool HasSpecularProfile() const;

	FSubstrateMaterialComplexity GetHighestComplexity() const;
#endif

	float DielectricSpecularToF0(float SpecularIn)
	{
		return 0.08f * SpecularIn;
	};
	
	//~ End UMaterialExpression Interface
};
