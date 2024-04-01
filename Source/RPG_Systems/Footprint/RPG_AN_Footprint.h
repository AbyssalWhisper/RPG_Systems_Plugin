

#pragma once

#include "CoreMinimal.h"
#include  "RPG_Systems/Footprint/RPG_FootprintData.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RPG_AN_Footprint.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_AN_Footprint : public UAnimNotify
{
public:
	URPG_AN_Footprint();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	    URPG_FootprintData* FootprintData;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName Socket;
private:
	GENERATED_BODY()
};
