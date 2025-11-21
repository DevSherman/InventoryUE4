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
    UI->ShowMouseSlot(bMouseHasItem);

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

void UInventoryComponent::OnMouseButtonPressed(EMouseButton button)
{
    if (CurrentItemSlotID < 0) return;

    switch (button)
    {
        case LEFT:          LeftClickEvent();                   break;
        case RIGHT:         /*contextual menu ?*/               break;
        case MIDDLE:        MiddleClickEvent();                break;
        case SCROLL_DOWN:   ScrollEvent(EMoveType::DECREASE);    break;
        case SCROLL_UP:     ScrollEvent(EMoveType::INCREASE);    break;

        default:                                                break;
    }
}

void UInventoryComponent::OnMouseButtonRelease(EMouseButton button)
{
    if(button == EMouseButton::RIGHT) return;

    if (button == EMouseButton::MIDDLE)
    {
        if (SplitSlotIDArray.Num() > 0)
        {
            SplitSlotIDArray.Empty();
            MouseSlotID_Cache = -1;
        }

        bOnSplitMode = false;
        return;
    }

    if (button == EMouseButton::LEFT)
    {
        if (CurrentItemSlotID > -1)
        {
            if (bMouseHasItem) TryDropStackToSlot();
            else
            {
                //if (CurrentContainerID > -1) //return to original slot
                //else //drop mouseSlot
            }
        }
        return;
    }
}

TSubclassOf<class UItemSlotUI> UInventoryComponent::GetItemSlotUI() const
{
    return  BP_ItemSlotUI;
}

void UInventoryComponent::LoadInventory()
{

}

void UInventoryComponent::SaveInventory()
{

}

void UInventoryComponent::UpdateCurrentItemSlotSelected(int ID)
{
    CurrentItemSlotID = ID;
    return;

    if (bOnSplitMode && ID != MouseSlotID_Cache)
    {
        if (ItemStackArray[ID].HasItem()) return;
        if(SplitSlotIDArray.AddUnique(ID) > -1) SplitCurrentStack();
    }
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
    if (SlotID < 0) return;

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

void UInventoryComponent::LeftClickEvent()
{
    bool slotHasItem = ItemStackArray[CurrentItemSlotID].HasItem();

    if (bMouseHasItem)
    {
        if (slotHasItem) SwapSlots();
        else DropMouseSlotToCurrentSlot();
    }
    else if (slotHasItem) MoveStackToMouseSlot();
}

void UInventoryComponent::MiddleClickEvent()
{
    if (bMouseHasItem) //spread
    {
        SpreadMode();
    }
    else
    {
        bOnSplitMode = true;
        FItemStack currentStack = ItemStackArray[CurrentItemSlotID];
        if (!currentStack.bCanStack && currentStack.Count <= 1) return;
        SplitItemStackCount = currentStack.Count;
        SplitCurrentStack();
    }
}

void UInventoryComponent::RightClickEvent()
{

}

void UInventoryComponent::ScrollEvent(EMoveType MoveType)
{

}

void UInventoryComponent::TryDropStackToSlot()
{
    if (CurrentSlotHasItem())
    {
        if (SlotsHasSameItem()) StackMouseSlotToCurrentSlot();
        else SwapSlots();
    }
    else DropMouseSlotToCurrentSlot();
}

void UInventoryComponent::MoveStackToMouseSlot()
{
    SetMouseSlot(ItemStackArray[CurrentItemSlotID]);
    ClearSlot(CurrentItemSlotID);
}

void UInventoryComponent::StackMouseSlotToCurrentSlot()
{
    FItemStack currentStack = ItemStackArray[CurrentItemSlotID];

    if (!currentStack.bCanStack) SwapSlots();
    else
    {
        int sum = currentStack.Count + MouseItemStack.Count;
        if (sum <= currentStack.MaxStack)
        {
            UpdateItemStackCount(CurrentItemSlotID, sum);
            ClearMouseSlot();
        }
        else SwapSlots();
    }
}

void UInventoryComponent::SetMouseSlot(FItemStack ItemStack)
{
    bMouseHasItem = true;
    MouseItemStack = ItemStack;
    if(!bOnSplitMode) MouseSlotID_Cache = CurrentItemSlotID;
    UI->UpdateMouseSlot(LoadIcon(ItemStack.StringID), ItemStack.Count);
    UI->ShowMouseSlot(true);
}

void UInventoryComponent::ClearSlot(int ID)
{
    if (ID < 0) return;

    ItemStackArray[ID].Clear();
    ItemSlotUIArray[ID]->Clear();
}

void UInventoryComponent::ClearMouseSlot()
{
    //MouseItemStack = FItemStack();
    UI->ShowMouseSlot(false);
    bMouseHasItem = false;
    bOnSplitMode = false;
    MouseSlotID_Cache = -1;
}

void UInventoryComponent::SwapSlots()
{
    SetItemStackSlot(MouseSlotID_Cache, ItemStackArray[CurrentItemSlotID]);
    SetItemStackSlot(CurrentItemSlotID, MouseItemStack);
    ClearMouseSlot();
}

void UInventoryComponent::DropMouseSlotToCurrentSlot()
{
    SetItemStackSlot(CurrentItemSlotID, MouseItemStack);

    if (MouseSlotID_Cache != CurrentItemSlotID && !bOnSplitMode)  ClearSlot(MouseSlotID_Cache);
    ClearMouseSlot();
}

void UInventoryComponent::SplitCurrentStack()
{    
    if (SplitSlotIDArray.Num() == 0)
    {
        FVector2D splitAmount = SplitStack(SplitItemStackCount, 2);
        UpdateItemStackCount(CurrentItemSlotID, splitAmount.X);

        FItemStack currentStack = ItemStackArray[CurrentItemSlotID];
        currentStack.Count = splitAmount.Y;
        SetMouseSlot(currentStack);
    }
}

void UInventoryComponent::SpreadMode()
{
    if (SplitSlotIDArray.Num() > 0)
    {
        FVector2D splitAmount;
        ClearMouseSlot();

        if (SplitSlotIDArray.Num() == 1)
        {
            splitAmount = SplitStack(SplitItemStackCount, 2);
            FItemStack currentStack = MouseItemStack;
            SetItemStackSlot(SplitSlotIDArray[0], currentStack);
            UpdateItemStackCount(SplitSlotIDArray[0], splitAmount.X);

            UpdateItemStackCount(CurrentItemSlotID, splitAmount.Y);
        }
        else
        {
            splitAmount = SplitStack(SplitItemStackCount, SplitSlotIDArray.Num());

            for (int i = 0; i < SplitSlotIDArray.Num() - 2; i++)
            {
                FItemStack currentStack = MouseItemStack;
                SetItemStackSlot(SplitSlotIDArray[i], currentStack);
                UpdateItemStackCount(SplitSlotIDArray[i], splitAmount.X);
            }

            UpdateItemStackCount(SplitSlotIDArray[SplitSlotIDArray.Num() - 1], splitAmount.Y);
        }
    }
}

FVector2D UInventoryComponent::SplitStack(int ItemStackCount, int SlotCount)
{
    int splitAmount = ItemStackCount / SlotCount;
    return FVector2D(splitAmount + (ItemStackCount % 2 == 0 ? 0 : 1), splitAmount);
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
