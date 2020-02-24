// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "TicTacToeGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

ATicTacToeGameModeBase::ATicTacToeGameModeBase()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_PlayerPawn"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}