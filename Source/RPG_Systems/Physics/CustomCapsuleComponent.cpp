#include "CustomCapsuleComponent.h"
#include "PhysicsEngine/BodySetup.h"

UCustomCapsuleComponent::UCustomCapsuleComponent()
{
	CapsuleCenterOffset = FVector::ZeroVector;
}

void UCustomCapsuleComponent::SetCapsuleCenter(FVector NewCenter)
{
	CapsuleCenterOffset = NewCenter;
	UpdateBodySetup(); // Atualiza a colisão
}
template <EShapeBodySetupHelper UpdateBodySetupAction, typename BodySetupType>
bool InvalidateOrUpdateCapsuleBodySetup(BodySetupType& ShapeBodySetup, bool bUseArchetypeBodySetup, float CapsuleRadius, float CapsuleHalfHeight)
{
	check((bUseArchetypeBodySetup && UpdateBodySetupAction == EShapeBodySetupHelper::InvalidateSharingIfStale) || (!bUseArchetypeBodySetup && UpdateBodySetupAction == EShapeBodySetupHelper::UpdateBodySetup));
	check(ShapeBodySetup->AggGeom.SphylElems.Num() == 1);
	FKSphylElem* SE = ShapeBodySetup->AggGeom.SphylElems.GetData();

	const float Length = 2 * FMath::Max(CapsuleHalfHeight - CapsuleRadius, 0.f);	//SphylElem uses height from center of capsule spheres, but UCapsuleComponent uses halfHeight from end of the sphere

	if (UpdateBodySetupAction == EShapeBodySetupHelper::UpdateBodySetup)
	{
		SE->SetTransform(FTransform::Identity);
		SE->Center = FVector(0, 0, Length * 0.5f + CapsuleRadius);
		SE->Radius = CapsuleRadius;
		SE->Length = Length;
	}
	else
	{
		if(SE->Radius != CapsuleRadius || SE->Length != Length)
		{
			ShapeBodySetup = nullptr;
			bUseArchetypeBodySetup = false;
		}
	}
	
	return bUseArchetypeBodySetup;
}
void UCustomCapsuleComponent::UpdateBodySetup()
{
	if (PrepareSharedBodySetup<UCapsuleComponent>())
	{
		bUseArchetypeBodySetup = InvalidateOrUpdateCapsuleBodySetup<EShapeBodySetupHelper::InvalidateSharingIfStale>(ShapeBodySetup, bUseArchetypeBodySetup, CapsuleRadius, CapsuleHalfHeight);
	}

	CreateShapeBodySetupIfNeeded<FKSphylElem>();

	if (!bUseArchetypeBodySetup)
	{
		InvalidateOrUpdateCapsuleBodySetup<EShapeBodySetupHelper::UpdateBodySetup>(ShapeBodySetup, bUseArchetypeBodySetup, CapsuleRadius, CapsuleHalfHeight);
	}
	MarkRenderStateDirty();
}

FPrimitiveSceneProxy* UCustomCapsuleComponent::CreateSceneProxy()
{
  class FDrawCylinderSceneProxy final : public FPrimitiveSceneProxy
  {
  public:
    SIZE_T GetTypeHash() const override
    {
      static size_t UniquePointer;
      return reinterpret_cast<size_t>(&UniquePointer);
    }

    FDrawCylinderSceneProxy(const UCustomCapsuleComponent* InComponent)
      : FPrimitiveSceneProxy(InComponent),
        bDrawOnlyIfSelected(InComponent->bDrawOnlyIfSelected),
        CapsuleRotation(InComponent->GetComponentQuat()),
        CapsuleRadius(InComponent->GetScaledCapsuleRadius()),
        CapsuleHalfHeight(InComponent->GetScaledCapsuleHalfHeight()),
        ShapeColor(InComponent->ShapeColor)
    {
      bWillEverBeLit = false;
    }

    void GetDynamicMeshElements(
      const TArray<const FSceneView*>& Views,
      const FSceneViewFamily& ViewFamily,
      uint32 VisibilityMap,
      FMeshElementCollector& Collector
    ) const override
    {
      const FMatrix& LocalToWorld = GetLocalToWorld();
      const int32 CapsuleSides = FMath::Clamp<int32>(CapsuleRadius / 4.f, 16, 64);
      for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
      {
        if (VisibilityMap & (1 << ViewIndex))
        {
          const FSceneView* View = Views[ViewIndex];
          const FLinearColor DrawCapsuleColor = GetViewSelectionColor(
            ShapeColor,
            *View,
            IsSelected(),
            IsHovered(),
            false,
            IsIndividuallySelected()
          );
          FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
          DrawWireCapsule(
            PDI,
            LocalToWorld.GetOrigin() + LocalToWorld.GetUnitAxis(EAxis::Z) * CapsuleHalfHeight,
            LocalToWorld.GetUnitAxis(EAxis::X),
            LocalToWorld.GetUnitAxis(EAxis::Y),
            LocalToWorld.GetUnitAxis(EAxis::Z),
            DrawCapsuleColor,
            CapsuleRadius,
            CapsuleHalfHeight,
            CapsuleSides,
            SDPG_World
          );
        }
      }
    }

    FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
    {
      const bool bProxyVisible = !bDrawOnlyIfSelected || IsSelected();
      const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();
      FPrimitiveViewRelevance Result;
      Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
      Result.bDynamicRelevance = true;
      Result.bShadowRelevance = IsShadowCast(View);
      Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
      return Result;
    }

    uint32 GetMemoryFootprint(void) const override
    {
      return (sizeof(*this) + GetAllocatedSize());
    }

    uint32 GetAllocatedSize(void) const
    {
      return (FPrimitiveSceneProxy::GetAllocatedSize());
    }

  private:
    const uint32 bDrawOnlyIfSelected:1;
    const FRotator CapsuleRotation;
    const float CapsuleRadius;
    const float CapsuleHalfHeight;
    const FColor ShapeColor;
  };

  return new FDrawCylinderSceneProxy(this);
}