// Copyright Epic Games, Inc. All Rights Reserved.

#include "BetterUtilitiesBPLibrary.h"
#include "BetterUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBetterUtilitiesBPLibrary::UBetterUtilitiesBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{ 
}

float UBetterUtilitiesBPLibrary::BetterUtilitiesSampleFunction(float Param)
{
	return -1;
}
 
EPlatform UBetterUtilitiesBPLibrary::GetPlatformEnum()
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
 

void UBetterUtilitiesBPLibrary::ClearWidget(UUserWidget*& Widget)
{
	if (Widget)
	{
		Widget->RemoveFromParent();
		Widget = nullptr;
	}
}

void UBetterUtilitiesBPLibrary::GetPlatform(EPlatform& Platform)
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


void UBetterUtilitiesBPLibrary::RunInEditor(ETrueFalse& TrueFalse)
{
#if WITH_EDITOR
    TrueFalse = ETrueFalse::True_;
    return;
#endif
    TrueFalse = ETrueFalse::False_;
}

 
FTimerHandle UBetterUtilitiesBPLibrary::EditorDelay(FTimerDynamicDelegate InputDelegate, float DelayTime)
{
    FTimerHandle TimerHandle;
    if (!InputDelegate.IsBound())return TimerHandle;
#if WITH_EDITOR
    GEditor->GetTimerManager()->SetTimer(TimerHandle, InputDelegate, DelayTime, false);
  
#endif
  return TimerHandle;
}


void UBetterUtilitiesBPLibrary::ClearTimerInEditor(FTimerHandle TimerHandle)
{
#if WITH_EDITOR
    if (!GEditor)
    {
        return;
    }
 
    GEditor->GetTimerManager()->ClearTimer(TimerHandle);
#endif

}

void UBetterUtilitiesBPLibrary::SetMaterials(UMeshComponent* TargetMesh,
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

void UBetterUtilitiesBPLibrary::SetSkeletalMeshAndMaterials(USkeletalMeshComponent* TargetMesh, USkeletalMesh* Mesh,
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

void UBetterUtilitiesBPLibrary::SetStaticMeshAndMaterials(UStaticMeshComponent* TargetMesh, UStaticMesh* Mesh,
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

void UBetterUtilitiesBPLibrary::AddWidgetToClientViewport(UWidget* Widget, int Z_Order)
{
    if (GEngine->GetCurrentPlayWorld() && Widget)
    {
       UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld())->GetGameViewportClient()->AddViewportWidgetContent(Widget->TakeWidget(), Z_Order);
    }
}

UObject* UBetterUtilitiesBPLibrary::GetWorldContextObjectFromGEngine()
{
   return GEngine->GetCurrentPlayWorld();
}

FString UBetterUtilitiesBPLibrary::Conv_GameplayTagToString(FGameplayTag Tag)
{
    return Tag.ToString();
}


UAnimMontage* UBetterUtilitiesBPLibrary::AutoLoadAnimMontage(TSoftObjectPtr<UAnimMontage> SoftAnimMontage)
{
    return SoftAnimMontage.LoadSynchronous();
}

TSubclassOf<UGameplayAbility> UBetterUtilitiesBPLibrary::AutoLoadGameplayAbility(
    TSoftClassPtr<UGameplayAbility> SoftGameplayAbility)
{
    return SoftGameplayAbility.LoadSynchronous();
}
 

USkeletalMesh* UBetterUtilitiesBPLibrary::AutoLoadSkeletalMesh(TSoftObjectPtr<USkeletalMesh> SoftSkeletalMesh)
{
    return SoftSkeletalMesh.LoadSynchronous();
}

UStaticMesh* UBetterUtilitiesBPLibrary::AutoLoadStaticMesh(TSoftObjectPtr<UStaticMesh> SoftStaticMesh)
{
    return SoftStaticMesh.LoadSynchronous();
}

void UBetterUtilitiesBPLibrary::ShowMouseCursor(bool bShow)
{
    APlayerController* PC = GEngine->GetCurrentPlayWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->bShowMouseCursor = bShow;
    }
}

bool UBetterUtilitiesBPLibrary::SimpleLineTraceSingleByChannel(const UObject* WorldContextObject,FVector StartLocation, FRotator Direction,
    double Distance, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore,
    EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor,
    FLinearColor TraceHitColor, float DrawTime)
{
    FVector EndLocation = StartLocation + (Direction.Vector() * Distance);
    
    return UKismetSystemLibrary::LineTraceSingle(WorldContextObject, StartLocation, EndLocation, TraceChannel,
        bTraceComplex, ActorsToIgnore, DrawDebugType,OutHit , bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}

 
 
