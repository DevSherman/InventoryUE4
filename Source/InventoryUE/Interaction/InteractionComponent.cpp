#include "InteractionComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "../Item/ItemActor.h"

void UInteractionComponent::Update(USceneComponent* Origin)
{
	FVector Start = Origin->GetComponentLocation();
	FVector End;
	if(TraceDirection == EDirection::FORWARD) End = Start + Origin->GetForwardVector() * Range;
	else End = Start + Origin->GetRightVector() * Range;
	
	CurrentActor = LineTraceSingle(Start, End);
	if (CurrentActor != nullptr)
	{
		AItemActor* Current = Cast<AItemActor>(CurrentActor);
		if (Current != nullptr)
		{
			if (Current != ItemActor)
			{
				if (ItemActor != nullptr) ItemActor->ShowOutline(false);
				ItemActor = Current;
				ItemActor->ShowOutline(true);
			}
		}
		else
		{
			if (ItemActor != nullptr) ItemActor->ShowOutline(false);
			ItemActor = nullptr;
		}

		if (bDEBUG)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("CurrentActor: %s"), *CurrentActor->GetName()));
		}
	}
	else if (ItemActor != nullptr)
	{
		ItemActor->ShowOutline(false);
		ItemActor = nullptr;
	}
}

void UInteractionComponent::Clear()
{
	//ItemActor->DestroyActor();
	ItemActor = nullptr;
}

AActor* UInteractionComponent::LineTraceSingle(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQueryParams;

	CollisionQueryParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByObjectType
	(
		OUT HitResult,
		Start,
		End,
		CollisionParams,
		CollisionQueryParams
	))
	{
		return HitResult.GetActor();
	}
	else nullptr;

	return nullptr;
}

