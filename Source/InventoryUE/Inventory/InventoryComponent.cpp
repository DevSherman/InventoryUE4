#include "InventoryComponent.h"
#include "InventoryUI.h"
#include "InventoryContainerUI.h"
#include "../Item/ItemActor.h"
#include "Utls.h"
#include <Kismet/GameplayStatics.h>

UInventoryComponent::UInventoryComponent()
{
    W_InventoryUI = Utls::LoadBlueprintFromPath<UInventoryUI>("WidgetBlueprint'/Game/Widgets/W_InventoryUI.W_InventoryUI'");
    PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (W_InventoryUI == nullptr) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] W_InventoryUI not found.")); return; }
    if (InventorySize.X * InventorySize.Y == 0) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] InventorySize bad size.")); return; }
    if (ToolBarSize.X * ToolBarSize.Y == 0) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] ToolBarSize bad size.")); return; }

    //Background Inventory and MouseSlot
    UI = CreateWidget<UInventoryUI>(GetWorld(), W_InventoryUI);
    if (!UI) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] UI not initilizaed.")); return; }
    UI->Init(*this);
    UI->AddToViewport(0);
    UI->ShowMouseSlot(bMouseItemSlotVisible);

    if (Padding <= 0) Padding = 1.0f;

    //TOOLBAR set & slots add
    int ToolBarWidth = (ToolBarSize.X * SlotUISize) + ((ToolBarSize.X - 1) * Padding);
    UI->GetToolBar()->Build(*this, 0, ToolBarSize.X, ToolBarSize.Y, FVector2D(ToolBarWidth, SlotUISize), Padding, true);
    UI->GetToolBar()->SetPosition(ToolBarPos);
    UI->GetToolBar()->Show(true);

    //INVENTORY set & slots add
    InventorySize = InventorySize.IntPoint();
    int InventoryHeight = (InventorySize.Y * SlotUISize) + ((InventorySize.Y - 1) * Padding);
    int InventoryWidth = (InventorySize.X * SlotUISize) + ((InventorySize.X - 1) * Padding);
    UI->GetInventory()->Build(*this, 1, InventorySize.X, InventorySize.Y, FVector2D(InventoryWidth, InventoryHeight), Padding, false);
    UI->GetInventory()->SetPosition(InventoryPos);
    UI->GetInventory()->Show(false);
}

void UInventoryComponent::SetCurrentItemSlotSelected(int ID)
{
    SelectedItemSlotID = ID;
}

void UInventoryComponent::SetCurrentContainerSelected(int ID)
{
    CurrentContainerID = ID;
}

void UInventoryComponent::RegisterItemSlotUI(UItemSlotUI& ItemSlotUI)
{
    ItemSlotsUI.Add(&ItemSlotUI);
    ItemStacks.Add(FItemStack());
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    FVector2D MousePos;
    if (PlayerController->GetMousePosition(MousePos.X, MousePos.Y))
        UI->UpdateMousePos(MousePos);
}

void UInventoryComponent::ShowHideUI()
{
    bInventoryOpened = !bInventoryOpened;
    UI->GetInventory()->Show(bInventoryOpened);

    if (bInventoryOpened)
    {
        PlayerController->StopMovement();
        PlayerController->SetInputMode(FInputModeUIOnly());
        PlayerController->SetShowMouseCursor(true);
    }
    else
    {
        PlayerController->SetShowMouseCursor(false);
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}
