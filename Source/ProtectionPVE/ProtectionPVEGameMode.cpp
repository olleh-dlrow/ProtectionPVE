// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProtectionPVEGameMode.h"
#include "ProtectionPVECharacter.h"
#include "UObject/ConstructorHelpers.h"

AProtectionPVEGameMode::AProtectionPVEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
