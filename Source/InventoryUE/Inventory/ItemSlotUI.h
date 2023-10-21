

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotUI.generated.h"

UCLASS()
class INVENTORYUE_API UItemSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(class UInventoryComponent& Inventory, int SlotID, bool IsToolBar = false);
	void Update(class UTexture2D* IconImage, int Amount);
	void UpdateCount(int Amount);
	void Clear();

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

public:
	int ID = -1;

protected:
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultUIState = FLinearColor::White;
	UPROPERTY(EditAnywhere)
	FLinearColor SelectedUIState = FLinearColor::Gray;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BackgroundImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CountBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* IndexBlock;

private:
	class UInventoryComponent* Inventory;
};
