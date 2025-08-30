// Copyright Epic Games, Inc. All Rights Reserved.

#include "BetterUtilitiesBPLibrary.h"
#include "BetterUtilities.h"
#include "GameplayTagContainer.h"
#include "BetterUtilities/Objects/ReplicatedObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"


DEFINE_LOG_CATEGORY(LogRPG_Systems);

UBetterUtilities::UBetterUtilities(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{ 
}

float UBetterUtilities::BetterUtilitiesSampleFunction(float Param)
{
	return -1;
}
 
EPlatform UBetterUtilities::GetPlatformEnum()
{
    FString PlatformName = UGameplayStatics::GetPlatformName();

    if (PlatformName.Equals(TEXT("Windows"), ESearchCase::IgnoreCase))
    {
        return EPlatform::Windows;
    }
    else if (PlatformName.Equals(TEXT("Mac"), ESearchCase::IgnoreCase))
    {
        return EPlatform::Mac;
    }
    else if (PlatformName.Equals(TEXT("Linux"), ESearchCase::IgnoreCase))
    {
        return EPlatform::Linux;
    }
    else if (PlatformName.Equals(TEXT("Android"), ESearchCase::IgnoreCase))
    {
        return EPlatform::Android;
    }
    else if (PlatformName.Equals(TEXT("IOS"), ESearchCase::IgnoreCase))
    {
        return EPlatform::iOS;
    }
    else if (PlatformName.Equals(TEXT("PS4"), ESearchCase::IgnoreCase))
    {
        return EPlatform::PS4;
    }
    else if (PlatformName.Equals(TEXT("XboxOne"), ESearchCase::IgnoreCase))
    {
        return EPlatform::XboxOne;
    }
    else if (PlatformName.Equals(TEXT("Switch"), ESearchCase::IgnoreCase))
    {
        return EPlatform::Switch;
    }
    else
    {
        return EPlatform::Other;
    }
}

ERotationDirection UBetterUtilities::YawRotationDirection(FRotator BaseRotation, FRotator TargetRotation, float Angle)
{
    FRotator rotation = UKismetMathLibrary::NormalizedDeltaRotator(BaseRotation, TargetRotation);
    if (rotation.Yaw > Angle)
    {
        return ERotationDirection::Left;
    }
    if (rotation.Yaw < -Angle)
    {
        return ERotationDirection::Right;
    }
    return ERotationDirection::Center;
}


void UBetterUtilities::ClearWidget(UUserWidget*& Widget)
{
	if (Widget)
	{
		Widget->RemoveFromParent();
		Widget = nullptr;
	}
}

void UBetterUtilities::GetPlatform(EPlatform& Platform)
{
    FString PlatformName = UGameplayStatics::GetPlatformName();
    
    if (PlatformName.Equals(TEXT("Windows"), ESearchCase::IgnoreCase))
    {
        Platform = EPlatform::Windows;
    }
    else if (PlatformName.Equals(TEXT("Mac"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::Mac;
    }
    else if (PlatformName.Equals(TEXT("Linux"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::Linux;
    }
    else if (PlatformName.Equals(TEXT("Android"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::Android;
    }
    else if (PlatformName.Equals(TEXT("IOS"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::iOS;
    }
    else if (PlatformName.Equals(TEXT("PS4"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::PS4;
    }
    else if (PlatformName.Equals(TEXT("XboxOne"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::XboxOne;
    }
    else if (PlatformName.Equals(TEXT("Switch"), ESearchCase::IgnoreCase))
    {
        Platform =  EPlatform::Switch;
    }
    else
    {
        Platform =  EPlatform::Other;
    }
}


void UBetterUtilities::RunInEditor(ETrueFalse& TrueFalse)
{
#if WITH_EDITOR
    TrueFalse = ETrueFalse::True_;
#else
    TrueFalse = ETrueFalse::False_;
#endif
}

float UBetterUtilities::GetPercentFromAttribute(const UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayAttribute Attribute, const FGameplayAttribute MaxAttribute)
{
    const float MaxValue = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);
    const float CurrentValue = AbilitySystemComponent->GetNumericAttribute(Attribute);
    return GetPercent(CurrentValue, MaxValue);
}

float UBetterUtilities::GetAlphaPercentFromAttribute(const UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayAttribute Attribute, const FGameplayAttribute MaxAttribute)
{
    const float MaxValue = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);
    const float CurrentValue = AbilitySystemComponent->GetNumericAttribute(Attribute);
    return GetAlphaPercent(CurrentValue, MaxValue);
}


FTimerHandle UBetterUtilities::EditorDelay(FTimerDynamicDelegate InputDelegate, float DelayTime)
{
    FTimerHandle TimerHandle;
    if (!InputDelegate.IsBound())return TimerHandle;
#if WITH_EDITOR
    GEditor->GetTimerManager()->SetTimer(TimerHandle, InputDelegate, DelayTime, false);
  
#endif
  return TimerHandle;
}


void UBetterUtilities::ClearTimerInEditor(FTimerHandle TimerHandle)
{
#if WITH_EDITOR
    if (!GEditor)
    {
        return;
    }
 
    GEditor->GetTimerManager()->ClearTimer(TimerHandle);
#endif

}

void UBetterUtilities::SetMaterials(UMeshComponent* TargetMesh,
    TArray<UMaterialInterface*> Materials)
{
    if (TargetMesh)
    {
        for (int i = 0; i < Materials.Num(); ++i)
        {
            TargetMesh->SetMaterial(i,Materials[i]);
        }
    }
}

void UBetterUtilities::SetSkeletalMeshAndMaterials(USkeletalMeshComponent* TargetMesh, USkeletalMesh* Mesh,
    TArray<UMaterialInterface*> Materials)
{
    if (TargetMesh)
    {
        TargetMesh->SetSkeletalMesh(Mesh);
        for (int i = 0; i < Materials.Num(); ++i)
        {
            TargetMesh->SetMaterial(i,Materials[i]);
        }
    }
}

void UBetterUtilities::SetStaticMeshAndMaterials(UStaticMeshComponent* TargetMesh, UStaticMesh* Mesh,
    TArray<UMaterialInterface*> Materials)
{
    if (TargetMesh)
    {
        TargetMesh->SetStaticMesh(Mesh);
        for (int i = 0; i < Materials.Num(); ++i)
        {
            TargetMesh->SetMaterial(i,Materials[i]);
        }
    }
}

void UBetterUtilities::AddWidgetToClientViewport(UWidget* Widget, int Z_Order)
{
    if (GEngine->GetCurrentPlayWorld() && Widget)
    {
       UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld())->GetGameViewportClient()->AddViewportWidgetContent(Widget->TakeWidget(), Z_Order);
    }
}

UObject* UBetterUtilities::GetWorldContextObjectFromGEngine()
{
   return GEngine->GetCurrentPlayWorld();
}

FString UBetterUtilities::Conv_GameplayTagToString(FGameplayTag Tag)
{
    return Tag.ToString();
}


UAnimMontage* UBetterUtilities::AutoLoadAnimMontage(TSoftObjectPtr<UAnimMontage> SoftAnimMontage)
{
    return SoftAnimMontage.LoadSynchronous();
}

TSubclassOf<UGameplayAbility> UBetterUtilities::AutoLoadGameplayAbility(
    TSoftClassPtr<UGameplayAbility> SoftGameplayAbility)
{
    return SoftGameplayAbility.LoadSynchronous();
}
 

USkeletalMesh* UBetterUtilities::AutoLoadSkeletalMesh(TSoftObjectPtr<USkeletalMesh> SoftSkeletalMesh)
{
    return SoftSkeletalMesh.LoadSynchronous();
}

UStaticMesh* UBetterUtilities::AutoLoadStaticMesh(TSoftObjectPtr<UStaticMesh> SoftStaticMesh)
{
    return SoftStaticMesh.LoadSynchronous();
}

UMaterialInterface* UBetterUtilities::AutoLoadMaterial(TSoftObjectPtr<UMaterialInterface> Material)
{
    return Material.LoadSynchronous();
}

TArray<UMaterialInterface*> UBetterUtilities::AutoLoadMaterials(TArray<TSoftObjectPtr<UMaterialInterface>> Materials)
{
    TArray<UMaterialInterface*> a;
    for (auto A : Materials)
    {
        a.Add(A.LoadSynchronous());
    }
    return a;
}

void UBetterUtilities::ShowMouseCursor(bool bShow)
{
    APlayerController* PC = GEngine->GetCurrentPlayWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->bShowMouseCursor = bShow;
    }
}

bool UBetterUtilities::SimpleLineTraceSingleByChannel(const UObject* WorldContextObject,FVector StartLocation, FRotator Direction,
    double Distance, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore,
    EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor,
    FLinearColor TraceHitColor, float DrawTime)
{
    FVector EndLocation = StartLocation + (Direction.Vector() * Distance);
    
    return UKismetSystemLibrary::LineTraceSingle(WorldContextObject, StartLocation, EndLocation, TraceChannel,
        bTraceComplex, ActorsToIgnore, DrawDebugType,OutHit , bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}


FVector UBetterUtilities::GetRandomPointInRadius(FVector Location,float Radius)
{
    return Location + (FMath::VRand()*(FMath::FRand()*Radius));
}

FVector UBetterUtilities::GetRandomPointInBoxCollision(UBoxComponent* BoxCollision)
{
    // Verifique se a caixa de colisão é válida
    if (!BoxCollision)
    {
        UE_LOG(LogTemp, Warning, TEXT("Box collision is null"));
        return FVector::ZeroVector;
    }

    // Obtenha a localização e os vetores da caixa de colisão
    FVector BoxLocation = BoxCollision->GetComponentLocation();
    FVector BoxExtent = BoxCollision->GetScaledBoxExtent();
    FVector BoxUpVector = BoxCollision->GetUpVector();
    FVector BoxForwardVector = BoxCollision->GetForwardVector();
    FVector BoxRightVector = BoxCollision->GetRightVector();

    // Calcule um ponto aleatório dentro da caixa de colisão local
    FVector RandomLocation = BoxLocation +
        (FMath::FRandRange(-1.f, 1.f) * BoxExtent.X * BoxForwardVector) +
        (FMath::FRandRange(-1.f, 1.f) * BoxExtent.Y * BoxRightVector) +
        (FMath::FRandRange(-1.f, 1.f) * BoxExtent.Z * BoxUpVector);

    return RandomLocation;
}

FVector UBetterUtilities::GetRandomPointInSphereCollision(USphereComponent* SphereCollision)
{
    // Verifique se a colisão esférica é válida
    if (!SphereCollision)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sphere collision is null"));
        return FVector::ZeroVector;
    }

    // Obtenha a localização e o raio da colisão esférica
    FVector SphereLocation = SphereCollision->GetComponentLocation();
    float SphereRadius = SphereCollision->GetScaledSphereRadius();

    // Gere uma direção aleatória
    FVector RandomDirection = FMath::VRand();

    // Calcule a localização aleatória dentro da colisão esférica
    FVector RandomLocation = SphereLocation + RandomDirection * (FMath::FRand()*SphereRadius);

    return RandomLocation;
}

void UBetterUtilities::DebugLog(FString LogMessage, EEasylog LogVerbosity, bool PrintScreen)
{
    FString FinalLogString = LogMessage;
    switch (LogVerbosity) {
    case NoLogging:
		
        break;
    case Fatal:
        UE_LOG(LogRPG_Systems,Fatal, TEXT("%s"), *FinalLogString)
        break;
    case Error:
        UE_LOG(LogRPG_Systems,Error, TEXT("%s"), *FinalLogString)
        break;
    case Warning:
        UE_LOG(LogRPG_Systems,Warning, TEXT("%s"), *FinalLogString)
        break;
    case Log:
        UE_LOG(LogRPG_Systems, Log, TEXT("%s"), *FinalLogString)
        break;
    }
    if (PrintScreen) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FinalLogString);
}

float UBetterUtilities::GetDeltaSecondsFromStepMs(const float& StepMs)
{
    return StepMs * 0.001f;
}

void UBetterUtilities::AddReplicatedSubObject(UActorComponent* ActorComponent, UObject* Object, ELifetimeCondition LifetimeCondition)
{
    if (!ActorComponent && !Object)return;
    ActorComponent->AddReplicatedSubObject(Object, LifetimeCondition);
}


void UBetterUtilities::RemoveReplicatedSubObject(UActorComponent* ActorComponent, UObject* Object)
{
    if (!ActorComponent && !Object)return;
    ActorComponent->RemoveReplicatedSubObject(Object);
}



TArray<UClass*> UBetterUtilities::GetAllDerivedClasses(UClass* ParentClass)
{
    // trace scope
    TRACE_CPUPROFILER_EVENT_SCOPE(GetAllDerivedClasses);
    FString ParentClassTagName;
    //log all class functions
    if (ParentClass)
    {
        DebugLog(ParentClass->GetClass()->GetFullName(), EEasylog::Warning);
        TSoftClassPtr<UObject> SoftClass = TSoftClassPtr<UObject>(ParentClass);
        
      
        ParentClassTagName = FString::Printf(TEXT("%s'%s'"), *ParentClass->GetClass()->GetClassPathName().ToString(), *SoftClass->GetPathName());
        DebugLog(FString::Printf(TEXT("Class Tag Name: %s"), *ParentClassTagName), EEasylog::Warning);
    }
    
    TArray<UClass*> Result;
    if (!ParentClass) return Result;

    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AR = ARM.Get();
    

    FARFilter Filter;
    Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
    Filter.TagsAndValues.Add("ParentClass", ParentClassTagName);
    //Filter.bRecursivePaths = true;
    // NÃO adiciona PackagePaths -> busca global

    TArray<FAssetData> Assets;
    AR.GetAssets(Filter, Assets);
    
    for (const FAssetData& Asset : Assets)
    {
        FString ClassPath;
        if (Asset.GetTagValue("GeneratedClass", ClassPath))
            //Asset.GetTagValue("GeneratedClassPath", ClassPath))
        {
            
            if (UClass* LoadedClass = LoadObject<UClass>(nullptr, *ClassPath))
            {
                Result.Add(LoadedClass);
                auto ChildClasses = GetAllDerivedClasses(LoadedClass);
                Result.Append(ChildClasses);
            }
        }
        
    }

    return Result;
}

TArray<TSoftClassPtr<UObject>> UBetterUtilities::GetAllDerivedClasses_SoftClass(TSubclassOf<UObject> ParentClass)
{
    // trace scope
    TRACE_CPUPROFILER_EVENT_SCOPE(GetAllDerivedClasses_SoftClass);
    FString ParentClassTagName;
    //log all class functions
    if (ParentClass)
    {
        UBetterUtilities::DebugLog(ParentClass->GetClass()->GetFullName(), EEasylog::Warning);
        
        TSoftClassPtr<UObject> SoftClass = TSoftClassPtr<UObject>(ParentClass);
        
      
        ParentClassTagName = FString::Printf(TEXT("%s'%s'"), *ParentClass->GetClass()->GetClassPathName().ToString(), *SoftClass->GetPathName());
        DebugLog(FString::Printf(TEXT("Class Tag Name: %s"), *ParentClassTagName), EEasylog::Warning);
    }
    
    TArray<TSoftClassPtr<UObject>> Result;
    if (!ParentClass) return Result;

    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AR = ARM.Get();
    

    FARFilter Filter;
    Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
    Filter.TagsAndValues.Add("ParentClass", ParentClassTagName);
    //Filter.bRecursivePaths = true;
    // NÃO adiciona PackagePaths -> busca global

    TArray<FAssetData> Assets;
    AR.GetAssets(Filter, Assets);
    
    for (const FAssetData& Asset : Assets)
    {
        FString ClassPath;
        if (Asset.GetTagValue("GeneratedClass", ClassPath))
            //Asset.GetTagValue("GeneratedClassPath", ClassPath))
        {
            DebugLog(FString::Printf(TEXT("Class Path: %s"), *ClassPath), EEasylog::Warning);
            
            auto LoadedClass = TSoftClassPtr<UClass>(FSoftObjectPath(ClassPath));
            
            Result.Add(LoadedClass);
            TArray<TSoftClassPtr<UObject>> ChildClasses = GetAllDerivedClassesFromAssetTag(ClassPath);
            Result.Append(ChildClasses);
        }
        
    }

    return Result;
}

TArray<TSoftClassPtr<UObject>> UBetterUtilities::GetAllDerivedClassesFromAssetTag(FString ParentClassPath)
{
    FString ParentClassTagName;
    //log all class functions

    ParentClassTagName = FString::Printf(TEXT("%s"), *ParentClassPath);
    DebugLog(FString::Printf(TEXT("Class Tag Name: %s"), *ParentClassTagName), EEasylog::Warning);
    
    
    TArray<TSoftClassPtr<UObject>> Result;

    FAssetRegistryModule& ARM = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AR = ARM.Get();
    

    FARFilter Filter;
    Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
    Filter.TagsAndValues.Add("ParentClass", ParentClassTagName);
    //Filter.bRecursivePaths = true;
    // NÃO adiciona PackagePaths -> busca global

    TArray<FAssetData> Assets;
    AR.GetAssets(Filter, Assets);
    
    for (const FAssetData& Asset : Assets)
    {
        FString ClassPath = FString();
        if (Asset.GetTagValue("GeneratedClass", ClassPath))
            //Asset.GetTagValue("GeneratedClassPath", ClassPath))
        {
            DebugLog(FString::Printf(TEXT("Class Path: %s"), *ClassPath), EEasylog::Warning);
            auto LoadedClass = TSoftClassPtr<UClass>(FSoftObjectPath(ClassPath));
            Result.Add(LoadedClass);
            auto ChildClasses = GetAllDerivedClassesFromAssetTag(ClassPath);
            Result.Append(ChildClasses);
        }
        
    }

    return Result;
}

TArray<TSoftObjectPtr<UWorld>> UBetterUtilities::GetAllMaps()
{
    TArray<TSoftObjectPtr<UWorld>> MapList;

    // Carrega o módulo do AssetRegistry
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    // Configura o filtro para buscar mapas (UWorld)
    FARFilter Filter;
    Filter.ClassPaths.Add(UWorld::StaticClass()->GetClassPathName());
    Filter.bRecursiveClasses = true;
    Filter.bRecursivePaths = true;

    // Se quiser apenas mapas do jogo (exclui mapas da Engine)
    // Filter.PackagePaths.Add("/Game");

    TArray<FAssetData> Assets;
    AssetRegistry.GetAssets(Filter, Assets);

    for (const FAssetData& Asset : Assets)
    {
        FString AssetPath = Asset.GetSoftObjectPath().ToString(); // Ex: /Game/Maps/MyMap.MyMap
        // Ignorar mapas gerados automaticamente
        
        if (AssetPath.Contains("_Generated_"))
            continue;
        TSoftObjectPtr<UWorld> SoftMap(AssetPath);
        MapList.Add(SoftMap);
        DebugLog(FString::Printf(TEXT("Found Map: %s"), *AssetPath), EEasylog::Log);
    }

    return MapList;
}

TArray<UDataAsset*> UBetterUtilities::GetAllDataAssetsOfClass_BP(TSubclassOf<UDataAsset> DataAssetClass)
{
    //Get assets from registry
    TArray<UDataAsset*> DataAssets;
    if (!DataAssetClass)return TArray<UDataAsset*>();
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    TArray<FAssetData> AssetData;
    AssetRegistry.GetAssetsByClass(DataAssetClass->GetClassPathName(), AssetData,true);
    for (const FAssetData& Data : AssetData)
    {
        UDataAsset* DataAsset = Cast<UDataAsset>(Data.GetAsset());
        if (DataAsset)
        {
            DataAssets.Add(DataAsset);
        }
    }
    return DataAssets;
}


#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "IPlatformFilePak.h"
#include "Misc/PackageName.h"

