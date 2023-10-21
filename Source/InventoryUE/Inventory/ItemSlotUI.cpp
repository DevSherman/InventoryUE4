#include "ItemSlotUI.h"
#include "InventoryUI.h"

#include "Engine/Texture2D.h" 
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryComponent.h"


void UItemSlotUI::Init(UInventoryComponent& _Inventory, int SlotID, bool bShowSlotID)
{
	Inventory = &_Inventory;
	ID = SlotID;
	if (bShowSlotID) IndexBlock->SetText(FText::AsNumber(ID + 1));
}

void UItemSlotUI::Update(UTexture2D* IconImage, int Amount)
{
	ItemImage->SetBrushFromSoftTexture(IconImage);
	UpdateCount(Amount);
}

void UItemSlotUI::UpdateCount(int Amount)
{
	CountBlock->SetText(FText::AsNumber(Amount));
}

void UItemSlotUI::Clear()
{
	ItemImage->Brush = FSlateBrush();
	CountBlock->SetText(FText::FromString(""));
}

void UItemSlotUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Inventory->SetCurrentItemSlotSelected(ID);
	BackgroundImage->SetColorAndOpacity(SelectedUIState);
}

void UItemSlotUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Inventory->SetCurrentItemSlotSelected(-1);
	BackgroundImage->SetColorAndOpacity(DefaultUIState);
}


