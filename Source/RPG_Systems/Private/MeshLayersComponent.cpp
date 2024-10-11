// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshLayersComponent.h"
#include "BetterUtilitiesBPLibrary.h"

// Sets default values for this component's properties
UMeshLayersComponent::UMeshLayersComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeshLayersComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeshLayersComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UStaticMeshComponent* UMeshLayersComponent::AddStaticMeshLayer(UMeshComponent* OwnerMesh,UStaticMesh* Mesh,FGameplayTag LayerTag,FMeshLayerPreset Preset)
{


	if (OwnerMesh && Mesh)
	{
		//rever


		
		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(OwnerMesh);
		
		MeshComponent->RegisterComponent();
		MeshComponent->SetStaticMesh(Mesh);
		auto& MeshLayersList= Meshs.FindOrAdd(OwnerMesh).MeshLayersList;//.FindOrAdd(Tag,gg);
		
		if (MeshLayersList.Contains(LayerTag))
		{

			UMeshComponent*& ref = MeshLayersList.FindChecked(LayerTag);
			if (ref)
			{
				ref->DestroyComponent();
				ref = MeshComponent;
			}
		}
		else
		{
			MeshLayersList.Add(LayerTag,MeshComponent);
			
		}
		MeshComponent->AttachToComponent(OwnerMesh,Preset.AttachmentTransformRules,Preset.Socket);
		MeshComponent->SetCollisionEnabled(Preset.Collision);
		return MeshComponent;
	}
	return nullptr;
}

UStaticMeshComponent* UMeshLayersComponent::AddStaticMeshLayerFromSoftObject(UMeshComponent* OwnerMesh, TSoftObjectPtr<UStaticMesh> Mesh, FGameplayTag LayerTag, FMeshLayerPreset Preset)
{
	return AddStaticMeshLayer(OwnerMesh,Mesh.LoadSynchronous(),LayerTag,Preset);
}

USkeletalMeshComponent* UMeshLayersComponent::AddSkeletalMeshLayer(UMeshComponent* OwnerMesh, USkeletalMesh* Mesh, FGameplayTag LayerTag, FMeshLayerPreset Preset)
{
	if (OwnerMesh && Mesh)
	{
		USkeletalMeshComponent* MeshComponent = NewObject<USkeletalMeshComponent>(OwnerMesh);

		if (MeshComponent)
		{
			MeshComponent->RegisterComponent();
			MeshComponent->SetSkeletalMesh(Mesh);

			// Encontra ou adiciona a lista de camadas de malha para o OwnerMesh
			auto& MeshLayersList = Meshs.FindOrAdd(OwnerMesh).MeshLayersList;

			// Verifica se já existe uma malha na camada especificada
			if (MeshLayersList.Contains(LayerTag))
			{
				UMeshComponent*& Ref = MeshLayersList.FindChecked(LayerTag);
				if (Ref)
				{
					// Destroi a malha existente e substitui pelo novo componente
					Ref->DestroyComponent();
					Ref = MeshComponent;
				}
			}
			else
			{
				// Adiciona o componente de malha à lista da camada
				MeshLayersList.Add(LayerTag, MeshComponent);
			}

			
			// Configura a colisão do componente de malha
			MeshComponent->SetCollisionEnabled(Preset.Collision);
			auto* SkeletalMesh = Cast<USkeletalMeshComponent>(OwnerMesh);
			if(SkeletalMesh)
			{
				if (Preset.UsePoseMaster)
				{
					MeshComponent->SetLeaderPoseComponent(SkeletalMesh,false);
					// Anexa o componente de malha ao OwnerMesh
					MeshComponent->AttachToComponent(OwnerMesh, Preset.AttachmentTransformRules);

				}else
				{
					// Anexa o componente de malha ao OwnerMesh
					MeshComponent->AttachToComponent(OwnerMesh, Preset.AttachmentTransformRules, Preset.Socket);
				}
			}
			
			
			return MeshComponent;
		}
	}
	return nullptr;
}

USkeletalMeshComponent* UMeshLayersComponent::AddSkeletalMeshLayerFromSoftObject(UMeshComponent* OwnerMesh, TSoftObjectPtr<USkeletalMesh> Mesh, FGameplayTag LayerTag, FMeshLayerPreset Preset)
{
	return AddSkeletalMeshLayer(OwnerMesh,Mesh.LoadSynchronous(),LayerTag,Preset);
}

USkeletalMeshComponent* UMeshLayersComponent::AddAsycSoftSkeletalMeshLayer(UMeshComponent* OwnerMesh, TSoftObjectPtr<USkeletalMesh> Mesh, FGameplayTag LayerTag, FMeshLayerPreset Preset)
{
	TWeakObjectPtr<UMeshLayersComponent> WeakMeshLayersComponent = this;
	UBetterUtilities::LoadAssetAsync<USkeletalMesh>(Mesh, [WeakMeshLayersComponent,OwnerMesh,Mesh,LayerTag,Preset](USkeletalMesh* LoadedMesh)
		{
			if (!WeakMeshLayersComponent.IsValid())return;
			if (LoadedMesh)
			{
				
				// Execute aqui o código após o asset ser carregado 
				WeakMeshLayersComponent->AddSkeletalMeshLayer(OwnerMesh, LoadedMesh, LayerTag, Preset);

			}
		});
	return nullptr;
}

void UMeshLayersComponent::RemoveMeshLayer(UMeshComponent* OwnerMesh, FGameplayTag LayerTag)
{
	if (!OwnerMesh) return;
	FMeshLayers* ref = Meshs.Find(OwnerMesh);
	if (!ref)return;
	if (auto* Mesh = ref->MeshLayersList.FindChecked(LayerTag))
	{
		if(Mesh) Mesh->DestroyComponent();
		ref->MeshLayersList.Remove(LayerTag);
	}
}

void UMeshLayersComponent::RemoveAllMeshLayers(UMeshComponent* OwnerMesh)
{
	if (!OwnerMesh) return;
	FMeshLayers* ref = Meshs.Find(OwnerMesh);
	if (!ref)return;
	for (auto var : ref->MeshLayersList)
	{
		if (var.Value) var.Value->DestroyComponent();
		ref->MeshLayersList.Remove(var.Key);
	}
	Meshs.Remove(OwnerMesh);
}



