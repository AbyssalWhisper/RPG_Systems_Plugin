// Copyright Epic Games, Inc. All Rights Reserved.

#include "RigUnit_MakeReferencePoseFromSkeletalMesh.h"

#include "AnimNextStats.h"
#include "DataRegistry.h"
#include "Engine/SkeletalMesh.h"
#include "UAFLogging.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RigUnit_MakeReferencePoseFromSkeletalMesh)

FRigUnit_MakeReferencePoseFromSkeletalMesh_Execute()
{
	SCOPE_CYCLE_COUNTER(STAT_AnimNext_Make_RefPose);

	using namespace UE::UAF;

	if(SkeletalMesh == nullptr)
	{
		UAF_RIGUNIT_LOG(Warning, TEXT("Could not make ref pose - Skeletal mesh is not valid."));
		return;
	}

	ReferencePose.ReferencePose = FDataRegistry::Get()->GetOrGenerateReferencePose(SkeletalMesh);
}
