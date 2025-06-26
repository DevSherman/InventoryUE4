#pragma once

#include "CoreMinimal.h"
#include "ItemStack.generated.h"

USTRUCT()
struct FItemStack
{
	GENERATED_BODY()

public:
	void SetCount(int Amount) { Count = Amount; }
	void UpdateCount(int Amount) { Count += Amount; }
	bool HasItem() const { return !StringID.IsEmpty(); }
	bool IsFull() const { return Count == MaxStack; }

	inline void Clear()
	{
		DisplayName = "";
		StringID = "";
		bCanStack = false;
		MaxStack = -1;
		Count = -1;
	}

public:
	UPROPERTY(EditAnywhere)
	FString DisplayName;
	UPROPERTY(EditAnywhere)
	FString StringID;
	UPROPERTY(EditAnywhere)
	class AActor* ItemActor;
	UPROPERTY(EditAnywhere)
	bool bCanStack = false;
	UPROPERTY(EditAnywhere)
	int MaxStack = 1;
	UPROPERTY(EditAnywhere)
	int Count = 0;
};
