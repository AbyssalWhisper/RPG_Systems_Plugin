// Copyright Zurichi

#pragma once

#include "CoreMinimal.h"
#include "BetterUtilities/Objects/ConditionObject.h"
#include "AndConditionObject.generated.h"

/**
 * 
 */
UCLASS()
class BETTERUTILITIES_API UAndConditionObject : public UConditionObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UConditionObject*> ConditionObjects;

	virtual bool CheckCondition_Implementation(AActor* Actor)override;
};
