


#include "RPG_Systems/Footprint/RPG_AN_Footprint.h"

#include "RPG_FootprintComponent.h"

URPG_AN_Footprint::URPG_AN_Footprint()
{
}

void URPG_AN_Footprint::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		auto Comp = MeshComp->GetOwner()->GetComponentByClass<URPG_FootprintComponent>();
		if (Comp)
		{
			
			Comp->TrySpawnFootprint(FootprintData,Socket);
		}
	}
}
