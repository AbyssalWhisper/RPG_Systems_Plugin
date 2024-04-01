

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPG_FootprintData.generated.h"

class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;
/**
 * 
 */
USTRUCT(BlueprintType)
struct  FSTR_RPG_FootprintSettings : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* FootMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* Particle;
	FSTR_RPG_FootprintSettings() {
		FootMaterial = nullptr;
		Sound = nullptr;
		Particle = nullptr;
	}

};

UCLASS()
class RPG_SYSTEMS_API URPG_FootprintData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URPG_FootprintData();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffSetScale = 1;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TMap<TEnumAsByte<EPhysicalSurface>, FSTR_RPG_FootprintSettings> FootprintData;

};
