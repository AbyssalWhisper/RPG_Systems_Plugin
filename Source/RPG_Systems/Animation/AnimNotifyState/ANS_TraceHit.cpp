// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_TraceHit.h"

#include "Kismet/KismetMathLibrary.h"
#include "RPG_Systems/TraceHit/BaseTraceHit.h"

UANS_TraceHit::UANS_TraceHit()
{
	NotifyColor = FColor::Red;
}

void UANS_TraceHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	LastTraceLocation = GetTraceLocation(MeshComp);
	ActorsToIgnore.Add(MeshComp->GetOwner());
	
}

void UANS_TraceHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	ExecuteTrace(MeshComp,Animation);
	
}

void UANS_TraceHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ActorsToIgnore.Empty();
}

void UANS_TraceHit::ExecuteTrace(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TArray<FHitResult> HitResults = TArray<FHitResult>();

	switch (TraceType) {
	case SphereTrace:
		UKismetSystemLibrary::SphereTraceMulti(MeshComp,LastTraceLocation,
		GetTraceLocation(MeshComp),Radius,TraceChannel,
			bTraceComplex,ActorsToIgnore,DrawDebugTrace,HitResults,true,
			TraceColor,TraceHitColor,TraceDuration);
		break;
	case BoxTrace:
		UKismetSystemLibrary::BoxTraceMulti(MeshComp,LastTraceLocation,
		GetTraceLocation(MeshComp),BoxExtend,
		MeshComp->GetSocketRotation(SocketName),TraceChannel,
			bTraceComplex,ActorsToIgnore,DrawDebugTrace,HitResults,true,
			TraceColor,TraceHitColor,TraceDuration);
		break;
	}
	
	LastTraceLocation = GetTraceLocation(MeshComp);

	for (auto Hit : HitResults)
	{
		ActorsToIgnore.AddUnique(Hit.GetActor());
	}
	if(HitResults.Num() > 0) OnHit(MeshComp,Animation,HitResults);
}

FVector UANS_TraceHit::GetTraceLocation(USkeletalMeshComponent* MeshComp)
{
	FVector L_OffsetLoc = UKismetMathLibrary::GetForwardVector(MeshComp->GetSocketRotation(SocketName)) * OffSetLocation.X ;
	L_OffsetLoc += UKismetMathLibrary::GetRightVector(MeshComp->GetSocketRotation(SocketName))* OffSetLocation.Y ;
	L_OffsetLoc += UKismetMathLibrary::GetUpVector(MeshComp->GetSocketRotation(SocketName))* OffSetLocation.Z ;

	return MeshComp->GetSocketLocation(SocketName) + L_OffsetLoc;
}
