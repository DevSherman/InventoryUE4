#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "ItemStack.generated.h"

USTRUCT()
struct FItemStack
{
	GENERATED_BODY()

public:
	void SetCount(int Amount) { Count = Amount; }
	void UpdateCount(int Amount) { Count += Amount; }
	bool HasItem() { return !StringID.IsEmpty(); }
	bool IsFull() { return Count == MaxStack; }

	bool operator== (FItemStack& other)
	{
		return StringID.Equals(other.StringID);
	}

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
	bool bCanStack = false;
	UPROPERTY(EditAnywhere)
	int MaxStack = 1;
	UPROPERTY(EditAnywhere)
	int Count = 0;
};
