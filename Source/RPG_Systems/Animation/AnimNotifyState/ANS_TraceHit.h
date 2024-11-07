// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/HitResult.h"
#include "ANS_TraceHit.generated.h"

/**
 * 
 */
class USkeletalMeshComponent;
class UAnimSequenceBase;

UENUM()
enum ETraceType : uint8
{
	SphereTrace,
	BoxTrace,
};

UCLASS()
class RPG_SYSTEMS_API UANS_TraceHit : public UAnimNotifyState
{
	
	GENERATED_BODY()
	public:
	UANS_TraceHit();

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Trace")
	FName SocketName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Trace")
	TEnumAsByte<ETraceType> TraceType = ETraceType::SphereTrace;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Trace", meta=(EditCondition="TraceType==ETraceType::SphereTrace"))
	float Radius = 5.0f;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Trace", meta=(EditCondition="TraceType==ETraceType::BoxTrace"))
	FVector BoxExtend = FVector(10.0f, 10.0f, 10.0f);
	UPROPERTY(blueprintReadWrite, EditAnywhere,Category="Trace")
	FVector OffSetLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(blueprintReadWrite, EditAnywhere,Category="Trace")
	bool bTraceComplex = false;

	
	UPROPERTY()
	TArray<AActor*> HitActors;
	UPROPERTY()
	FVector LastTraceLocation;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Trace")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
	UPROPERTY(blueprintReadWrite, EditAnywhere,Category="Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::ForOneFrame;
	UPROPERTY(blueprintReadWrite, EditAnywhere,Category="Trace")
	float TraceDuration = 0.2f;
	UPROPERTY(blueprintReadWrite, EditAnywhere,Category="Trace")
	FLinearColor TraceColor = FLinearColor::Red;
	UPROPERTY(blueprintReadWrite, EditAnywhere,Category="Trace")
	FLinearColor TraceHitColor = FLinearColor::Green;
	
	 virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	 virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)override;
	 virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)override;

	void ExecuteTrace(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation,UPARAM(ref)TArray<FHitResult>& Results) const;

	UFUNCTION()
	FVector GetTraceLocation(USkeletalMeshComponent * MeshComp);
};
