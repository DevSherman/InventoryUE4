
#include "InventoryContainerUI.h"
#include "Components/WrapBox.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "ItemSlotUI.h"
#include "InventoryComponent.h"
#include "Utls.h"

void UInventoryContainerUI::Build(UInventoryComponent& _Inventory, int ID, int RowCount, int ColumnCount, FVector2D Size, float _Padding, bool bShowSlotID)
{
	this->Inventory = &_Inventory;
	ContainerID = ID;

	BoxContainer->SetInnerSlotPadding(FVector2D(_Padding, _Padding));

	CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
	if (CanvasSlot) CanvasSlot->SetSize(Size);
	else { UE_LOG(LogTemp, Error, TEXT("[UInventoryContainerUI] CanvasSlot not found in parent Widget")); return; }

	for (int slotID = 0; slotID < (RowCount * ColumnCount); slotID++)
	{
		UItemSlotUI* ItemSlotUI = CreateWidget<UItemSlotUI>(GetWorld(), Inventory->GetItemSlotUI());
		ItemSlotUI->Init(*Inventory, slotID, bShowSlotID);

		BoxContainer->AddChild(ItemSlotUI);
		Inventory->RegisterItemSlotUI(*ItemSlotUI);
	}

	UE_LOG(LogTemp, Warning, TEXT("[UInventoryContainerUI] ContainerID: %d  builded."), ContainerID);
}

void UInventoryContainerUI::SetPosition(FVector2D Pos)
{
	CanvasSlot->SetPosition(Pos);
}

void UInventoryContainerUI::Show(bool bValue)
{
	this->SetVisibility(bValue ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInventoryContainerUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Inventory->SetCurrentContainerSelected(ContainerID);
}

void UInventoryContainerUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Inventory->SetCurrentContainerSelected(-1);
}
