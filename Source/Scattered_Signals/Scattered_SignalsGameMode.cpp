// Copyright Epic Games, Inc. All Rights Reserved.

#include "Scattered_SignalsGameMode.h"
#include "Scattered_SignalsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AScattered_SignalsGameMode::AScattered_SignalsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
