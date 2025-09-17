#include "GameViewportUserWidget.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "GameFramework/GameModeBase.h"

#define LOCTEXT_NAMESPACE "GameViewportUserWidget"

// UGameViewportUserWidgetSlot Implementation
UGameViewportUserWidgetSlot::UGameViewportUserWidgetSlot(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , HorizontalAlignment(HAlign_Fill)
    , VerticalAlignment(VAlign_Fill)
{
}

void UGameViewportUserWidgetSlot::SynchronizeProperties()
{
    if (SlotBox.IsValid())
    {
        SlotBox->SetHAlign(HorizontalAlignment);
        SlotBox->SetVAlign(VerticalAlignment);
    }
}

void UGameViewportUserWidgetSlot::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    SlotBox.Reset();
}

void UGameViewportUserWidgetSlot::BuildSlot(TSharedRef<SOverlay> Overlay)
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

// UGameViewportUserWidget Implementation
UGameViewportUserWidget::UGameViewportUserWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bIsGameRunning(false)
    , bIsGamePaused(false)
{
    bIsVariable = true;
}

void UGameViewportUserWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    
    for (UPanelSlot* Slot_ : Slots)
    {
        if (UGameViewportUserWidgetSlot* ViewportSlot = Cast<UGameViewportUserWidgetSlot>(Slot_))
        {
            ViewportSlot->SynchronizeProperties();
        }
    }
}

UClass* UGameViewportUserWidget::GetSlotClass() const
{
    return UGameViewportUserWidgetSlot::StaticClass();
}

void UGameViewportUserWidget::OnSlotAdded(UPanelSlot* InSlot)
{
    Super::OnSlotAdded(InSlot);
    
    if (MyWidget.IsValid())
    {
        MyWidget = RebuildWidget();
    }
}

void UGameViewportUserWidget::OnSlotRemoved(UPanelSlot* InSlot)
{
    Super::OnSlotRemoved(InSlot);
    
    if (MyWidget.IsValid())
    {
        MyWidget = RebuildWidget();
    }
}

UWidget* UGameViewportUserWidget::GetContent() const
{
    if (Slots.Num() > 0)
    {
        return Slots[0]->Content;
    }
    return nullptr;
}

void UGameViewportUserWidget::SetContent(UWidget* Content)
{
    ClearContent();
    
    if (Content)
    {
        UGameViewportUserWidgetSlot* NewSlot = Cast<UGameViewportUserWidgetSlot>(AddChild(Content));
        if (NewSlot)
        {
            NewSlot->HorizontalAlignment = HAlign_Fill;
            NewSlot->VerticalAlignment = VAlign_Fill;
        }
    }
}

void UGameViewportUserWidget::ClearContent()
{
    ClearChildren();
}

void UGameViewportUserWidget::StartGame()
{
    if (!bIsGameRunning)
    {
        if (UWorld* World = GetWorld())
        {
            // Iniciar o jogo
            World->BeginPlay();
            
            // Configurar o GameMode
            if (AGameModeBase* GameMode = World->GetAuthGameMode())
            {
                GameMode->StartPlay();
            }
            
            bIsGameRunning = true;
            bIsGamePaused = false;
        }
    }
}

void UGameViewportUserWidget::PauseGame()
{
    if (bIsGameRunning && !bIsGamePaused)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetFirstPlayerController()->SetPause(true);
            bIsGamePaused = true;
        }
    }
}

void UGameViewportUserWidget::ResumeGame()
{
    if (bIsGameRunning && bIsGamePaused)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetFirstPlayerController()->SetPause(false);
            bIsGamePaused = false;
        }
    }
}

void UGameViewportUserWidget::StopGame()
{
    if (bIsGameRunning)
    {
        if (UWorld* World = GetWorld())
        {
            // Parar o jogo
            if (AGameModeBase* GameMode = World->GetAuthGameMode())
            {
                //GameMode->EndPlay();
            }
            
            bIsGameRunning = false;
            bIsGamePaused = false;
        }
    }
}

bool UGameViewportUserWidget::IsGameRunning() const
{
    return bIsGameRunning;
}

bool UGameViewportUserWidget::IsGamePaused() const
{
    return bIsGamePaused;
}

TSharedRef<SWidget> UGameViewportUserWidget::RebuildWidget()
{
    if (IsDesignTime())
    {
        return SNew(STextBlock).Text(LOCTEXT("IsDesignTime", "Game Viewport (Design Time)"));
    }

    InitializeViewport();
    
    if (!ViewportWidget.IsValid())
    {
        return SNew(STextBlock).Text(LOCTEXT("ViewportError", "Erro ao criar viewport"));
    }

    // Criar overlay para conter a viewport e widgets
    TSharedRef<SOverlay> OverlayWidget = SNew(SOverlay);

    // Adicionar viewport como primeira camada
    OverlayWidget->AddSlot()
    [
        ViewportWidget.ToSharedRef()
    ];

    // Adicionar widgets filhos sobre a viewport
    for (UPanelSlot* PanelSlot : Slots)
    {
        if (UGameViewportUserWidgetSlot* ViewportSlot = Cast<UGameViewportUserWidgetSlot>(PanelSlot))
        {
            ViewportSlot->BuildSlot(OverlayWidget);
        }
    }
    
    return OverlayWidget;
}

void UGameViewportUserWidget::InitializeViewport()
{
    if (!GameViewport)
    {
        if (UWorld* World = GetWorld())
        {
            GameViewport = World->GetGameViewport();
        }
    }
    
    if (!ViewportWidget.IsValid() && GameViewport)
    {
        ViewportWidget = SNew(SGameViewportWidget, GameViewport, this);
    }
}

void UGameViewportUserWidget::SpawnCube(FVector Location, bool bEnablePhysics)
{
    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh)
    {
        SpawnStaticMeshActor(CubeMesh, Location, bEnablePhysics);
    }
}

void UGameViewportUserWidget::SpawnSphere(FVector Location, bool bEnablePhysics)
{
    UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMesh)
    {
        SpawnStaticMeshActor(SphereMesh, Location, bEnablePhysics);
    }
}

void UGameViewportUserWidget::SpawnCylinder(FVector Location, bool bEnablePhysics)
{
    UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder"));
    if (CylinderMesh)
    {
        SpawnStaticMeshActor(CylinderMesh, Location, bEnablePhysics);
    }
}

void UGameViewportUserWidget::SpawnCustomMesh(UStaticMesh* Mesh, FVector Location, bool bEnablePhysics)
{
    if (Mesh)
    {
        SpawnStaticMeshActor(Mesh, Location, bEnablePhysics);
    }
}

AActor* UGameViewportUserWidget::SpawnActorByClass(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotation)
{
    if (!ActorClass || !GetWorld())
        return nullptr;

    UWorld* World = GetWorld();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AActor* NewActor = World->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParams);

    if (NewActor)
    {
        SpawnedActors.Add(NewActor);
        OnObjectSpawned.Broadcast(NewActor);
    }

    return NewActor;
}

void UGameViewportUserWidget::SpawnStaticMeshActor(UStaticMesh* Mesh, const FVector& Location, bool bEnablePhysics)
{
    if (!Mesh || !GetWorld())
        return;

    UWorld* World = GetWorld();
    
    AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>();
    if (NewActor)
    {
        UStaticMeshComponent* MeshComp = NewActor->GetStaticMeshComponent();
        if (MeshComp)
        {
            MeshComp->SetStaticMesh(Mesh);
            
            if (bEnablePhysics && bEnablePhysicsSimulation)
            {
                MeshComp->SetSimulatePhysics(true);
                MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
                MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
                MeshComp->SetMassOverrideInKg(NAME_None, 1.0f, true);
            }
            
            FVector SpawnLocation = Location.IsZero() ? GetRandomSpawnLocation() : Location;
            NewActor->SetActorLocation(SpawnLocation);
            
            SpawnedActors.Add(NewActor);
            OnObjectSpawned.Broadcast(NewActor);
        }
    }
}

void UGameViewportUserWidget::ClearAllObjects()
{
    if (!GetWorld())
        return;

    UWorld* World = GetWorld();
    
    for (auto& ActorPtr : SpawnedActors)
    {
        if (ActorPtr.IsValid())
        {
            World->DestroyActor(ActorPtr.Get());
        }
    }
    
    SpawnedActors.Empty();
}

void UGameViewportUserWidget::SetPhysicsEnabled(bool bEnabled)
{
    bEnablePhysicsSimulation = bEnabled;
    
    if (UWorld* World = GetWorld())
    {
        World->bShouldSimulatePhysics = bEnabled;
    }
}

void UGameViewportUserWidget::SetCameraPosition(FVector Location, FRotator Rotation)
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (APawn* Pawn = PC->GetPawn())
            {
                Pawn->SetActorLocation(Location);
                Pawn->SetActorRotation(Rotation);
            }
        }
    }
}

int32 UGameViewportUserWidget::GetObjectCount() const
{
    return SpawnedActors.Num();
}

FVector UGameViewportUserWidget::GetRandomSpawnLocation() const
{
    return FVector(
        FMath::RandRange(-200.0f, 200.0f),
        FMath::RandRange(-200.0f, 200.0f),
        FMath::RandRange(200.0f, 500.0f)
    );
}

void UGameViewportUserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    if (ViewportWidget.IsValid())
    {
        ViewportWidget.Reset();
    }
    
    Super::ReleaseSlateResources(bReleaseChildren);
}

// SGameViewportWidget Implementation
void SGameViewportWidget::Construct(const FArguments& InArgs, UGameViewportClient* InGameViewport, UGameViewportUserWidget* InOwnerWidget)
{
    GameViewport = InGameViewport;
    OwnerWidget = InOwnerWidget;
    
    SViewport::FArguments ViewportArgs;
   // ViewportArgs._ViewportInterface = InGameViewport;
    
    SViewport::Construct(ViewportArgs);
}

#undef LOCTEXT_NAMESPACE
