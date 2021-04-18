// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlyingAISoldierGameMode.h"
#include "FlyingAISoldierCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFlyingAISoldierGameMode::AFlyingAISoldierGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
