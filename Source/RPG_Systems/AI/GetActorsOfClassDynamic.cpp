// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/AI/GetActorsOfClassDynamic.h"

#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GetActorsOfClassDynamic)

UGetActorsOfClassDynamic::UGetActorsOfClassDynamic(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UGetActorsOfClassDynamic::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();
    if (QueryOwner == nullptr)
    {
        return;
    }
	
    UWorld* World = GEngine->GetWorldFromContextObject(QueryOwner, EGetWorldErrorMode::LogAndReturnNull);
    if (World == nullptr)
    {
        return;
    }
    UBlackboardComponent* Blackboard = nullptr;
    GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Blue,"ssssss");
    if (QueryInstance.Owner.IsValid())
    {
        Blackboard = Cast<UBlackboardComponent>(QueryInstance.Owner.Get());
    }
    
    if (Blackboard == nullptr) return;
    
    if (SearchedActorClass == nullptr) return;
    
    GenerateOnlyActorsInRadius.BindData(QueryOwner, QueryInstance.QueryID);
    bool bUseRadius = GenerateOnlyActorsInRadius.GetValue();

    TArray<AActor*> MatchingActors;
    if (bUseRadius)
    {
        TArray<FVector> ContextLocations;
        QueryInstance.PrepareContext(SearchCenter, ContextLocations);

        SearchRadius.BindData(QueryOwner, QueryInstance.QueryID);
        const float RadiusValue = SearchRadius.GetValue();
        const float RadiusSq = FMath::Square(RadiusValue);

        for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(World, SearchedActorClass); ItActor; ++ItActor)
        {
            for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ++ContextIndex)
            {
                if (FVector::DistSquared(ContextLocations[ContextIndex], ItActor->GetActorLocation()) < RadiusSq)
                {
                    MatchingActors.Add(*ItActor);
                    break;
                }
            }
        }
    }
    else
    {	// If radius is not positive, ignore Search Center and Search Radius and just return all actors of class.
        for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(World, SearchedActorClass); ItActor; ++ItActor)
        {
            MatchingActors.Add(*ItActor);
        }
    }

    QueryInstance.AddItemData<UEnvQueryItemType_Actor>(MatchingActors);
}