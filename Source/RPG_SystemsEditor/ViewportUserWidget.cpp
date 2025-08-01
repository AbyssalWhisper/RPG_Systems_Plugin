#include "ViewportUserWidget.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"

#define LOCTEXT_NAMESPACE "ViewportUserWidget"

// UViewportUserWidgetSlot Implementation
UViewportUserWidgetSlot::UViewportUserWidgetSlot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , HorizontalAlignment(HAlign_Fill)
    , VerticalAlignment(VAlign_Fill)
{
}

void UViewportUserWidgetSlot::SynchronizeProperties()
{
    if (SlotBox.IsValid())
    {
        SlotBox->SetHAlign(HorizontalAlignment);
        SlotBox->SetVAlign(VerticalAlignment);
    }
}

void UViewportUserWidgetSlot::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    SlotBox.Reset();
}

void UViewportUserWidgetSlot::BuildSlot(TSharedRef<SOverlay> Overlay)
{
    SlotBox = SNew(SBox)
        .HAlign(HorizontalAlignment)
        .VAlign(VerticalAlignment);

    if (Content)
    {
        SlotBox->SetContent(Content->TakeWidget());
    }

    Overlay->AddSlot()
    [
        SlotBox.ToSharedRef()
    ];
}

// UViewportUserWidget Implementation
UViewportUserWidget::UViewportUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
}

void UViewportUserWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    
    // Sincronizar propriedades dos slots
    for (UPanelSlot* Slot_ : Slots)
    {
        if (UViewportUserWidgetSlot* ViewportSlot = Cast<UViewportUserWidgetSlot>(Slot))
        {
            ViewportSlot->SynchronizeProperties();
        }
    }
}

UClass* UViewportUserWidget::GetSlotClass() const
{
    return UViewportUserWidgetSlot::StaticClass();
}

void UViewportUserWidget::OnSlotAdded(UPanelSlot* InSlot)
{
    Super::OnSlotAdded(InSlot);
    
    // Reconstruir o widget quando um slot é adicionado
    if (MyWidget.IsValid())
    {
        MyWidget = RebuildWidget();
    }
}

void UViewportUserWidget::OnSlotRemoved(UPanelSlot* InSlot)
{
    Super::OnSlotRemoved(InSlot);
    
    // Reconstruir o widget quando um slot é removido
    if (MyWidget.IsValid())
    {
        MyWidget = RebuildWidget();
    }
}

UWidget* UViewportUserWidget::GetContent() const
{
    if (Slots.Num() > 0)
    {
        return Slots[0]->Content;
    }
    return nullptr;
}

void UViewportUserWidget::SetContent(UWidget* Content)
{
    ClearContent();
    
    if (Content)
    {
        UViewportUserWidgetSlot* NewSlot = Cast<UViewportUserWidgetSlot>(AddChild(Content));
        if (NewSlot)
        {
            NewSlot->HorizontalAlignment = HAlign_Fill;
            NewSlot->VerticalAlignment = VAlign_Fill;
        }
    }
}

void UViewportUserWidget::ClearContent()
{
    ClearChildren();
}



void UViewportUserWidget::SpawnActorByClass(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotation)
{
    if (!ActorClass || !PreviewScene.IsValid() || !PreviewScene->GetWorld())
        return;
    UWorld* World = PreviewScene->GetWorld();
    if (!World) return;
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AActor* NewActor = World->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParams);

    SpawnedActors.Add(NewActor);
}

// Implementação das funções de profile para combo box
TArray<FString> UViewportUserWidget::GetAllProfileNames() const
{
    TArray<FString> ProfileNames;
    
    UAssetViewerSettings* DefaultSettings = UAssetViewerSettings::Get();
    if (DefaultSettings)
    {
        for (const auto& Profile : DefaultSettings->Profiles)
        {
            ProfileNames.Add(Profile.ProfileName);
        }
    }
    
    // Se não houver profiles, adicionar um padrão
    if (ProfileNames.Num() == 0)
    {
        ProfileNames.Add(TEXT("Default"));
    }
    
    return ProfileNames;
}

int32 UViewportUserWidget::GetCurrentProfileIndex() const
{
    if (PreviewScene.IsValid())
    {
        return PreviewScene->GetCurrentProfileIndex();
    }
    return 0;
}

FString UViewportUserWidget::GetCurrentProfileName() const
{
    if (PreviewScene.IsValid())
    {
        UAssetViewerSettings* DefaultSettings = UAssetViewerSettings::Get();
        if (DefaultSettings)
        {
            int32 CurrentProfile = PreviewScene->GetCurrentProfileIndex();
            if (DefaultSettings->Profiles.IsValidIndex(CurrentProfile))
            {
                return DefaultSettings->Profiles[CurrentProfile].ProfileName;
            }
        }
    }
    return TEXT("Default");
}

void UViewportUserWidget::SetViewportProfileByName(const FString& ProfileName)
{
    if (!PreviewScene.IsValid())
        return;
        
    UAssetViewerSettings* DefaultSettings = UAssetViewerSettings::Get();
    if (DefaultSettings)
    {
        for (int32 i = 0; i < DefaultSettings->Profiles.Num(); i++)
        {
            if (DefaultSettings->Profiles[i].ProfileName == ProfileName)
            {
                PreviewScene->SetProfileIndex(i);
                break;
            }
        }
    }
}

int32 UViewportUserWidget::GetProfileCount() const
{
    UAssetViewerSettings* DefaultSettings = UAssetViewerSettings::Get();
    if (DefaultSettings)
    {
        return DefaultSettings->Profiles.Num();
    }
    return 0;
}

TSharedRef<SWidget> UViewportUserWidget::RebuildWidget()
{
    if (IsDesignTime()) 
    {
        return SNew(STextBlock).Text(LOCTEXT("IsDesignTime", "IsDesignTime"));
    }

    InitializeViewport();
    
    if (!ViewportWidget.IsValid())
    {
        return SNew(STextBlock).Text(LOCTEXT("ViewportError", "Erro ao criar viewport"));
    }

    // Criar um overlay que contém o viewport como fundo
    TSharedRef<SOverlay> OverlayWidget = SNew(SOverlay);

    // Adicionar o viewport como primeira camada (fundo)
    OverlayWidget->AddSlot()
    [
        ViewportWidget.ToSharedRef()
    ];

    // Adicionar combo box de profiles se habilitado
    if (bShowProfileComboBox)
    {
        InitializeProfileComboBox();
        
        if (ProfileComboBox.IsValid())
        {
            OverlayWidget->AddSlot()
            .HAlign(HAlign_Left)
            .VAlign(VAlign_Top)
            .Padding(ProfileComboBoxPosition.X, ProfileComboBoxPosition.Y, 0, 0)
            [
                SNew(SBox)
                .WidthOverride(ProfileComboBoxSize.X)
                .HeightOverride(ProfileComboBoxSize.Y)
                [
                    ProfileComboBox.ToSharedRef()
                ]
            ];
        }
    }

    // Adicionar widgets filhos sobre o viewport
    for (UPanelSlot* PanelSlot : Slots)
    {
        if (UViewportUserWidgetSlot* ViewportSlot = Cast<UViewportUserWidgetSlot>(PanelSlot))
        {
            ViewportSlot->BuildSlot(OverlayWidget);
        }
    }
    
    return OverlayWidget;
}

void UViewportUserWidget::InitializeViewport()
{
    if (!PreviewScene.IsValid())
    {
        // Criar a cena de preview
        PreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
        PreviewScene->SetFloorVisibility(bShowFloor, true);
        
        // Configurar física
        if (bEnablePhysicsSimulation && PreviewScene->GetWorld())
        {
            PreviewScene->GetWorld()->bShouldSimulatePhysics = true;
        }
    }
    
    if (!ViewportWidget.IsValid())
    {
        ViewportWidget = SNew(SViewportWidget, PreviewScene, this);
    }
}

void UViewportUserWidget::SpawnCube(FVector Location, bool bEnablePhysics)
{
    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh)
    {
        SpawnStaticMeshActor(CubeMesh, Location, bEnablePhysics);
    }
}

void UViewportUserWidget::SpawnSphere(FVector Location, bool bEnablePhysics)
{
    UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh)
    {
        SpawnStaticMeshActor(SphereMesh, Location, bEnablePhysics);
    }
}

void UViewportUserWidget::SpawnCylinder(FVector Location, bool bEnablePhysics)
{
    UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh)
    {
        SpawnStaticMeshActor(CylinderMesh, Location, bEnablePhysics);
    }
}

void UViewportUserWidget::SpawnCustomMesh(UStaticMesh* Mesh, FVector Location, bool bEnablePhysics)
{
    if (Mesh)
    {
        SpawnStaticMeshActor(Mesh, Location, bEnablePhysics);
    }
}

void UViewportUserWidget::SpawnStaticMeshActor(UStaticMesh* Mesh, const FVector& Location, bool bEnablePhysics)
{
    if (!Mesh || !PreviewScene.IsValid() || !PreviewScene->GetWorld())
        return;

    UWorld* World = PreviewScene->GetWorld();
    
    AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>();
    if (NewActor)
    {
        UStaticMeshComponent* MeshComp = NewActor->GetStaticMeshComponent();
        if (MeshComp)
        {
            MeshComp->SetStaticMesh(Mesh);
            
            // Configurar física se solicitado
            if (bEnablePhysics && bEnablePhysicsSimulation)
            {
                MeshComp->SetSimulatePhysics(true);
                MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
                MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
                
                // Adicionar massa para melhor simulação
                MeshComp->SetMassOverrideInKg(NAME_None, 1.0f, true);
            }
            
            // Definir posição
            FVector SpawnLocation = Location.IsZero() ? GetRandomSpawnLocation() : Location;
            NewActor->SetActorLocation(SpawnLocation);
            
            // Adicionar à lista
            SpawnedActors.Add(NewActor);
            
            // Broadcast do evento
            OnObjectSpawned.Broadcast(NewActor);
        }
    }
}

void UViewportUserWidget::ClearAllObjects()
{
    if (!PreviewScene.IsValid() || !PreviewScene->GetWorld())
        return;

    UWorld* World = PreviewScene->GetWorld();
    
    for (auto& ActorPtr : SpawnedActors)
    {
        if (ActorPtr.IsValid())
        {
            World->DestroyActor(ActorPtr.Get());
        }
    }
    
    SpawnedActors.Empty();
}

void UViewportUserWidget::SetPhysicsEnabled(bool bEnabled)
{
    bEnablePhysicsSimulation = bEnabled;
    
    if (PreviewScene.IsValid() && PreviewScene->GetWorld())
    {
        PreviewScene->GetWorld()->bShouldSimulatePhysics = bEnabled;
    }
    
    if (ViewportWidget.IsValid())
    {
        TSharedPtr<FViewportWidgetClient> Client = StaticCastSharedPtr<FViewportWidgetClient>(ViewportWidget->GetViewportClient());
        if (Client.IsValid())
        {
            Client->SetPhysicsEnabled(bEnabled);
        }
    }
}

void UViewportUserWidget::SetCameraPosition(FVector Location, FRotator Rotation)
{
    if (ViewportWidget.IsValid())
    {
        TSharedPtr<FViewportWidgetClient> Client = StaticCastSharedPtr<FViewportWidgetClient>(ViewportWidget->GetViewportClient());
        if (Client.IsValid())
        {
            Client->SetViewLocation(Location);
            Client->SetViewRotation(Rotation);
        }
    }
}

int32 UViewportUserWidget::GetObjectCount() const
{
    return SpawnedActors.Num();
}

FVector UViewportUserWidget::GetRandomSpawnLocation() const
{
    return FVector(
        FMath::RandRange(-200.0f, 200.0f),
        FMath::RandRange(-200.0f, 200.0f),
        FMath::RandRange(200.0f, 500.0f)
    );
}

// Implementação das funções do combo box de profiles
void UViewportUserWidget::InitializeProfileComboBox()
{
    if (!ProfileComboBox.IsValid())
    {
        // Limpar opções anteriores
        ProfileOptions.Empty();
        
        // Obter todos os nomes dos profiles
        TArray<FString> ProfileNames = GetAllProfileNames();
        
        // Converter para TSharedPtr<FString> para uso no combo box
        for (const FString& ProfileName : ProfileNames)
        {
            ProfileOptions.Add(MakeShareable(new FString(ProfileName)));
        }
        
        // Criar o combo box usando lambdas ao invés de delegates
        ProfileComboBox = SNew(SComboBox<TSharedPtr<FString>>)
            .OptionsSource(&ProfileOptions)
            .OnGenerateWidget_Lambda([](TSharedPtr<FString> InOption)
            {
                return SNew(STextBlock)
                    .Text(FText::FromString(InOption.IsValid() ? *InOption : TEXT("Invalid")));
            })
            .OnSelectionChanged_Lambda([this](TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
            {
                if (SelectedItem.IsValid() && SelectInfo != ESelectInfo::Direct)
                {
                    SetViewportProfileByName(*SelectedItem);
                }
            })
            [
                SNew(STextBlock)
                .Text_Lambda([this]()
                {
                    return FText::FromString(GetCurrentProfileName());
                })
            ];
        
        // Definir seleção inicial
        FString CurrentProfileName = GetCurrentProfileName();
        for (const auto& Option : ProfileOptions)
        {
            if (Option.IsValid() && *Option == CurrentProfileName)
            {
                ProfileComboBox->SetSelectedItem(Option);
                break;
            }
        }
    }
}
void UViewportUserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    // 1. Limpar o ProfileComboBox
    if (ProfileComboBox.IsValid())
    {
        ProfileComboBox.Reset();
    }
    ProfileOptions.Empty();

    // 2. Limpar o ViewportClient primeiro para quebrar referências circulares
    if (ViewportWidget.IsValid())
    {
        TSharedPtr<FViewportWidgetClient> Client = StaticCastSharedPtr<FViewportWidgetClient>(ViewportWidget->GetViewportClient());
        if (Client.IsValid())
        {
            // Desabilitar física e parar atualizações
            Client->SetPhysicsEnabled(false);
        }
    }
    
    // 3. Limpar todos os atores spawned antes de destruir a cena
    if (PreviewScene.IsValid() && PreviewScene->GetWorld())
    {
        UWorld* World = PreviewScene->GetWorld();
        
        for (auto& ActorPtr : SpawnedActors)
        {
            if (ActorPtr.IsValid())
            {
                World->DestroyActor(ActorPtr.Get());
            }
        }
        SpawnedActors.Empty();
    }
    
    // 4. Liberar o widget da viewport (TSharedPtr)
    if (ViewportWidget.IsValid())
    {
        ViewportWidget.Reset();
    }
    
    // 5. Liberar a PreviewScene (TSharedPtr)
    if (PreviewScene.IsValid())
    {
        PreviewScene.Reset();
    }
    
    // 6. Chamar o método da classe pai para liberar recursos filhos
    Super::ReleaseSlateResources(bReleaseChildren);
}

// SViewportWidget Implementation
void SViewportWidget::Construct(const FArguments& InArgs, TSharedPtr<FAdvancedPreviewScene> InPreviewScene, UViewportUserWidget* InOwnerWidget)
{
    PreviewScene = InPreviewScene;
    OwnerWidget = InOwnerWidget;
    
    SEditorViewport::Construct(SEditorViewport::FArguments());
}

TSharedRef<FEditorViewportClient> SViewportWidget::MakeEditorViewportClient()
{
    ViewportClient = MakeShareable(new FViewportWidgetClient(PreviewScene.Get(), SharedThis(this), OwnerWidget.Get()));
    return ViewportClient.ToSharedRef();
}

// FViewportWidgetClient Implementation
FViewportWidgetClient::FViewportWidgetClient(FAdvancedPreviewScene* InPreviewScene, const TSharedRef<SViewportWidget>& InViewport, UViewportUserWidget* InOwnerWidget)
    : FEditorViewportClient(nullptr, InPreviewScene, StaticCastSharedRef<SEditorViewport>(InViewport))
    , OwnerWidget(InOwnerWidget)
{
    // Configurações básicas do viewport
    SetViewMode(VMI_Lit);
    SetRealtime(true);
    
    // Configurar câmera inicial
    if (OwnerWidget.IsValid())
    {
        SetViewLocation(OwnerWidget->InitialCameraLocation);
        SetViewRotation(OwnerWidget->InitialCameraRotation);
    }
    
    // Configurar controles da câmera
    bSetListenerPosition = false;
    bUsingOrbitCamera = true;
    SetCameraSpeedSetting(4);
    
}

void FViewportWidgetClient::Tick(float DeltaSeconds)
{
    FEditorViewportClient::Tick(DeltaSeconds);
    
    // Atualizar física se habilitada
    if (bPhysicsEnabled && PreviewScene && PreviewScene->GetWorld())
    {
        PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
    }
}

void FViewportWidgetClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    FEditorViewportClient::Draw(View, PDI);
}

void FViewportWidgetClient::SetPhysicsEnabled(bool bEnabled)
{
    bPhysicsEnabled = bEnabled;
}

#undef LOCTEXT_NAMESPACE
