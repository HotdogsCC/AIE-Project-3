// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIEProject3GameMode.h"
#include "AIEProject3Character.h"
#include "UObject/ConstructorHelpers.h"

AAIEProject3GameMode::AAIEProject3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
