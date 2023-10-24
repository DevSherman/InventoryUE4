#include "InventoryUI.h"

#include "ItemSlotUI.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryComponent.h"
#include "Utls.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UInventoryUI::NativeConstruct()
{
	bIsFocusable = true;
}

FReply UInventoryUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Inventory->OnClick();
	return FReply::Handled();
}

FReply UInventoryUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Error, TEXT("[UInventoryUI] OnClickRelease"));
	Inventory->OnClickRelease();
	return FReply::Handled();
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

void UInventoryUI::UpdateMousePos()
{
	FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	MouseItemSlotUI_Slot->SetPosition(Pos);
}

void UInventoryUI::UpdateMouseSlot(UTexture2D* Icon, int Count)
{
	MouseItemSlotUI->Update(Icon, Count);
}


