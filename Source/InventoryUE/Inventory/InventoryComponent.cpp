#include "InventoryComponent.h"
#include "InventoryUI.h"
#include "InventoryContainerUI.h"
#include "../Item/ItemActor.h"
#include "ItemSlotUI.h"
#include "Utls.h"
#include <Kismet/GameplayStatics.h>

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!W_InventoryUI) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] WidgetBlueprint 'W_InventoryUI' not set.")); return; }
    if (!BP_ItemSlotUI) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] WidgetBlueprint 'W_ItemSlotUI' not set.")); return; };  

    if (InventorySize.X * InventorySize.Y == 0) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] InventorySize bad size.")); return; }
    if (ToolBarSize.X * ToolBarSize.Y == 0) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] ToolBarSize bad size.")); return; }

    //Background Inventory and MouseSlot
    UI = CreateWidget<UInventoryUI>(GetWorld(), W_InventoryUI);
    if (!UI) { UE_LOG(LogTemp, Error, TEXT("[UInventoryComponent: %s] UI not initilizaed.")); return; }

    UI->Init(*this);
    UI->AddToViewport(0);
    UI->ShowMouseSlot(bOnDrag);

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

void UInventoryComponent::OnClick(EMouseButton button)
{
    if (CurrentItemSlotID < 0) return;
    bool hasItem = ItemStackArray[CurrentItemSlotID].HasItem();

    switch (button)
    {
        case LEFT:
            if(hasItem) MoveStackToMouseSlot();
            UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::OnClick] LEFT"));
            break;
        case RIGHT:

            UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::OnClick] RIGHT"));
            break;
        case MIDDLE:

            UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::OnClick] MIDDLE"));
            break;
        case SCROLL_DOWN:

            UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::OnClick] SCROLL_DOWN"));
            break;
        case SCROLL_UP:

            UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent::OnClick] SCROLL_UP"));
            break;
        default: break;
    }
}

void UInventoryComponent::OnClickRelease()
{
    if (CurrentItemSlotID > -1)
    {
        if (bOnDrag && Cached_CurrentItemSlotID > -1)
        {
            if (ItemStackArray[Cached_CurrentItemSlotID].HasItem()) SwapSlots();
            else MoveCurrentStack();
        }
        else
        {
            //if (CurrentContainerID > -1) //return to original slot
            //else //drop mouseSlot
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[UInventoryUI] OnClickRelease"));
}

TSubclassOf<class UItemSlotUI> UInventoryComponent::GetItemSlotUI() const
{
    return  BP_ItemSlotUI;
}

inline void UInventoryComponent::SetCurrentItemSlotSelected(int ID)
{
    if (!bOnDrag) CurrentItemSlotID = ID;
    else Cached_CurrentItemSlotID = ID;

    if (GEngine)
        GEngine->AddOnScreenDebugMessage
        (
            -1, 1.0f, FColor::Green,
            FString::Printf(TEXT("CurrentItemSlotID: %d - Cached_CurrentItemSlotID: %d"),
            CurrentItemSlotID, Cached_CurrentItemSlotID)
        );
}

inline void UInventoryComponent::SetCurrentContainerSelected(int ID)
{
    if (!bOnDrag) CurrentContainerID = ID;
    else Cached_CurrentContainerID = ID;
}

void UInventoryComponent::RegisterItemSlotUI(UItemSlotUI& ItemSlotUI)
{
    int Index = ItemSlotUIArray.Add(&ItemSlotUI);
    ItemSlotUIArray[Index]->ID = Index; //inventory container displacement
    ItemStackArray.Add(FItemStack());

    //UE_LOG(LogTemp, Warning, TEXT("ItemSlotUIArray Index: %d ItemSlotUI ID: %d"), Index, ItemSlotUIArray[Index]->ID);
}

bool UInventoryComponent::AddItem(AItemActor* ItemActor)
{
    FItemStack ItemStack = ItemActor->BuildStack();

    return CheckAvaliableSlot(ItemStack);
}

bool UInventoryComponent::CheckAvaliableSlot(FItemStack ItemStack)
{
    for (int slotID = 0; slotID < ItemStackArray.Num(); slotID++) //stacking
    {
        if (ItemStackArray[slotID].HasItem() && !ItemStackArray[slotID].IsFull())
        {
            if (ItemStackArray[slotID].StringID.Equals(ItemStack.StringID))
            {
                int TotalCount = ItemStackArray[slotID].Count + ItemStack.Count;

                if (TotalCount <= ItemStackArray[slotID].MaxStack)
                {
                    ItemStackArray[slotID].SetCount(TotalCount);
                    UpdateItemStackCount(slotID, TotalCount);
                    return true;
                }
                else
                {
                    UpdateItemStackCount(slotID, ItemStackArray[slotID].MaxStack);

                    FItemStack NewStack = ItemStack;
                    int Remaining = TotalCount - ItemStackArray[slotID].MaxStack;
                    NewStack.SetCount(Remaining);

                    return CheckAvaliableSlot(NewStack);
                }
            }
        }
    }
    for (int slotID = 0; slotID < ItemStackArray.Num(); slotID++) //fisrt empty slot
    {
        if (!ItemStackArray[slotID].HasItem())
        {
            SetItemStackSlot(slotID, ItemStack);
            return true;
        }
    }
    return false;
}

void UInventoryComponent::SetItemStackSlot(int SlotID, FItemStack ItemStack)
{
    ItemStackArray[SlotID] = ItemStack;
    ItemSlotUIArray[SlotID]->Update(LoadIcon(ItemStack.StringID), ItemStack.Count);
}

UTexture2D* UInventoryComponent::LoadIcon(FString StringID)
{
    const FString _IconPath = IconsPath + StringID;
    UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(_IconPath)));
    if (Texture != nullptr) return Texture;
    else return IconNotFoundTexture;
}

void UInventoryComponent::UpdateItemStackCount(int SlotID, int Count)
{
    ItemStackArray[SlotID].SetCount(Count);
    ItemSlotUIArray[SlotID]->UpdateCount(Count);
}

void UInventoryComponent::MoveStackToMouseSlot()
{
    SetMouseSlot(ItemStackArray[CurrentItemSlotID]);
    ClearSlot();
}

void UInventoryComponent::SetMouseSlot(FItemStack ItemStack)
{
    MouseItemStack = ItemStack;
    UI->UpdateMouseSlot(LoadIcon(ItemStack.StringID), ItemStack.Count);
    UI->ShowMouseSlot(true);

    bOnDrag = true;
}

void UInventoryComponent::ClearSlot()
{
    //TODO: check container 
    ItemStackArray[CurrentItemSlotID].Clear();
    ItemSlotUIArray[CurrentItemSlotID]->Clear();
}

void UInventoryComponent::ClearMouseSlot()
{
    //MouseItemStack = FItemStack();
    //clear?
    UI->ShowMouseSlot(false);

    bOnDrag = false;
}

void UInventoryComponent::SwapSlots()
{
    SetItemStackSlot(CurrentItemSlotID, ItemStackArray[Cached_CurrentItemSlotID]);
    SetItemStackSlot(Cached_CurrentItemSlotID, MouseItemStack);
    ClearMouseSlot();
}

void UInventoryComponent::MoveCurrentStack()
{
    SetItemStackSlot(Cached_CurrentItemSlotID, MouseItemStack);
    ClearSlot();
    ClearMouseSlot();
    CurrentItemSlotID = Cached_CurrentItemSlotID;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    UI->UpdateMousePos();
}

void UInventoryComponent::ShowHideUI()
{
    bInventoryOpened = !bInventoryOpened;
    UI->GetInventory()->Show(bInventoryOpened);

    if (bInventoryOpened)
    {
        PlayerController->StopMovement();
        PlayerController->SetInputMode(FInputModeUIOnly().SetWidgetToFocus(UI->TakeWidget()));
        PlayerController->SetShowMouseCursor(true);
    }
    else
    {
        PlayerController->SetShowMouseCursor(false);
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}
