// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TicTacToeWidget.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_API UTicTacToeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateGame(int value, int index);


	UFUNCTION(BlueprintImplementableEvent)
		void Victory(int PlayerId);
};
