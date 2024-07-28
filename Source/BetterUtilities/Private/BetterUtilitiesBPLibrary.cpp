// Copyright Epic Games, Inc. All Rights Reserved.

#include "BetterUtilitiesBPLibrary.h"
#include "BetterUtilities.h"
#include "BetterUtilities/Objects/ReplicatedObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(EasyLog);

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

void UBetterUtilities::DebugLog(FString LogMessage, EEasylog LogVerbosity)
{
    FString FinalLogString = LogMessage;
    switch (LogVerbosity) {
    case NoLogging:
		
        break;
    case Fatal:
        UE_LOG(EasyLog,Fatal, TEXT("%s"), *FinalLogString)
        break;
    case Error:
        UE_LOG(EasyLog,Error, TEXT("%s"), *FinalLogString)
        break;
    case Warning:
        UE_LOG(EasyLog,Warning, TEXT("%s"), *FinalLogString)
        break;
    case Verbose:
        UE_LOG(EasyLog,Verbose, TEXT("%s"), *FinalLogString)
        break;
    }
}

float UBetterUtilities::GetDeltaSecondsFromStepMs(const float& StepMs)
{
    return StepMs * 0.001f;
}

void UBetterUtilities::AddReplicatedSubObject(UActorComponent* ActorComponent, UReplicatedObject* Object, ELifetimeCondition LifetimeCondition)
{
    if (!ActorComponent && !Object)return;
    ActorComponent->AddReplicatedSubObject(Object, LifetimeCondition);
}

void UBetterUtilities::RemoveReplicatedSubObject(UActorComponent* ActorComponent, UReplicatedObject* Object)
{
    if (!ActorComponent && !Object)return;
    ActorComponent->RemoveReplicatedSubObject(Object);
}

