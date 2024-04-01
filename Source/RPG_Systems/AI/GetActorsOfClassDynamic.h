// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <DataProviders/AIDataProvider.h>
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_BlueprintBase.h"
#include "GetActorsOfClassDynamic.generated.h"


UCLASS()
class RPG_SYSTEMS_API UGetActorsOfClassDynamic : public UEnvQueryGenerator_BlueprintBase
{
public:
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName BlackboardPropertyName;
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderFloatValue SearchRadius;

	/** context */
	UPROPERTY(EditAnywhere, Category=Generator)
	TSubclassOf<UEnvQueryContext> SearchCenter;

	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderBoolValue GenerateOnlyActorsInRadius;
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderBoolValue SearchSubClass;

	UPROPERTY(EditDefaultsOnly, Category=Generator, meta=(AllowAbstract))
	TSubclassOf<AActor> SearchedActorClass;

};
