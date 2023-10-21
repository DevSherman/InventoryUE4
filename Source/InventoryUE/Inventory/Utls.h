#pragma once

#include "CoreMinimal.h"
#include <Runtime/Engine/Public/EngineUtils.h>
#include "DrawDebugHelpers.h"

namespace Utls
{
	template <typename T>
	static UClass* LoadBlueprintFromPath(const FString Path)
	{
		UBlueprint* blueprint = Cast<UBlueprint>(StaticLoadObject(UObject::StaticClass(), NULL, *Path));
		if (!blueprint)
		{
			UE_LOG(LogTemp, Error, TEXT("[Can't find/load blueprint: %s]"), *FString(typeid(T).name()));
			return nullptr;
		}
		return (UClass*)(blueprint->GeneratedClass);
	}
}
