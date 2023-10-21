#include "InventoryUI.h"

#include "ItemSlotUI.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryComponent.h"
#include "Utls.h"

void UInventoryUI::NativeConstruct()
{
	bIsFocusable = true;
}

FReply UInventoryUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab)
		Inventory->ShowHideUI();

	return FReply::Handled();
}

void UInventoryUI::Init(UInventoryComponent& _Inventory)
{
	this->Inventory = &_Inventory;

	BP_ItemSlotUI = Utls::LoadBlueprintFromPath<UItemSlotUI>("WidgetBlueprint'/Game/Widgets/W_ItemSlotUI.W_ItemSlotUI'");
	if (!BP_ItemSlotUI) return;

	MouseItemSlotUI->SetVisibility(ESlateVisibility::HitTestInvisible);
	MouseItemSlotUI->Init(*Inventory, -2, false);
	MouseItemSlotUI_Slot = Cast<UCanvasPanelSlot>(MouseItemSlotUI->Slot);
	MouseItemSlotUI_Slot->SetZOrder(1);

	if (!MouseItemSlotUI_Slot) UE_LOG(LogTemp, Error, TEXT("[UInventoryUI] Failed try get Slot"));
}

void UInventoryUI::ShowMouseSlot(bool bValue)
{
	MouseItemSlotUI->SetRenderOpacity(bValue ? 1.0f : 0.0f);
}

void UInventoryUI::UpdateMousePos(FVector2D Pos)
{
	MouseItemSlotUI_Slot->SetPosition(Pos);
}

UInventoryContainerUI* UInventoryUI::GetInventory()
{
	return InventoryContainerUI;
}

UInventoryContainerUI* UInventoryUI::GetToolBar()
{
	return ToolBarContainerUI;
}


