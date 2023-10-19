// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryUEGameMode.h"
#include "InventoryUEHUD.h"
#include "InventoryUECharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventoryUEGameMode::AInventoryUEGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AInventoryUEHUD::StaticClass();
}
