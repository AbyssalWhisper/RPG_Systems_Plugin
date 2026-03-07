// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UAF/Internal/AnimNextExecuteContext.h"
#include "UAF/Internal/Graph/AnimNext_LODPose.h"
#include "UAF/Internal/Graph/RigUnit_AnimNextBase.h"

#include "RigUnit_MakeReferencePoseFromSkeletalMesh.generated.h"

class USkeletalMesh;

/** Makes a reference pose from a skeletal mesh */
USTRUCT(meta=(DisplayName="Make Reference Pose (Skeletal Mesh)", Category="Animation Graph", NodeColor="0, 1, 1", Keywords="Output,Pose,Port"))
struct FRigUnit_MakeReferencePoseFromSkeletalMesh : public FRigUnit_AnimNextBase
{
	GENERATED_BODY()

	RIGVM_METHOD()
	void Execute();

	virtual FString GetUnitSubTitle() const { return TEXT("Skeletal Mesh"); };

	// Reference pose to write
	UPROPERTY(EditAnywhere, Category = "Graph", meta = (Input, Output))
	FAnimNextGraphReferencePose ReferencePose;

	// Mesh to use to generate the reference pose
	UPROPERTY(EditAnywhere, Category = "Graph", meta = (Input))
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	// The execution result
	UPROPERTY(EditAnywhere, DisplayName = "Execute", Category = "BeginExecution", meta = (Input, Output))
	FAnimNextExecuteContext ExecuteContext;
};
