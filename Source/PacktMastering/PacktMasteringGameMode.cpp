// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacktMasteringGameMode.h"
#include "PacktMasteringHUD.h"
#include "PacktMasteringCharacter.h"
#include "UObject/ConstructorHelpers.h"

APacktMasteringGameMode::APacktMasteringGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APacktMasteringHUD::StaticClass();
}
