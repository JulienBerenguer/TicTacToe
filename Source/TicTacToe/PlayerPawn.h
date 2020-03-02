// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "Math/IntVector.h"
#include "PlayerPawn.generated.h"

#define BOX_NUMBER 3

UCLASS()
class TICTACTOE_API APlayerPawn : public APawn
{
	GENERATED_BODY()

	// DEFAULT CODE
public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// VARIABLES
public:
	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		TSubclassOf<class UUserWidget> WidgetClass;

	// Variable to hold the widget After Creating it.
	UUserWidget* GameWidget;

	// List of tic tact toe boxes (0 = blank | 1 = cross | 2 = round)
	uint8 BoxList[BOX_NUMBER][BOX_NUMBER] = { 0 };

	// Player turn
	bool PlayerTurn = false;

	// Victory check - Check if all conditions are met
	bool VictoryList[BOX_NUMBER] = { false };


	// METHODS
public:
	UFUNCTION(BlueprintCallable)
		void TicTacToeGame(FIntVector ButtonPressed);

	UFUNCTION()
		bool VictoryCondition(uint8 PlayerId);
	UFUNCTION()
		bool VictoryScreen(uint8 PlayerId);
};
