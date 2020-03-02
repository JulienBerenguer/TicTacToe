// Fill out your copyright notice in the Description page of Project Settings.
//┌───┬───┬───┐
//│   │   │   │
//├───┼───┼───┤
//│   │   │   │
//├───┼───┼───┤
//│   │   │   │
//└───┴───┴───┘

#include "PlayerPawn.h"
#include "TicTacToeWidget.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass) // Check if the Asset is assigned in the blueprint.
	{
		// Automatique
		/*FStringClassReference MyWidgetClassRef(TEXT("/Game/Blueprints/Widgets/WBP_TicTacToe"));
		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			GameWidget = CreateWidget<UUserWidget>(GetWorld(), MyWidgetClass);
			// Do stuff with MyWidget World / GameInstance / PlayerController
		}*/

		// Create the widget and store it.
		//GameWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		GameWidget = CreateWidget<UTicTacToeWidget>(GetWorld(), WidgetClass);

		// L'initialise comme owner
		GameWidget->SetOwningPlayer(GetWorld()->GetFirstPlayerController());

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (GameWidget)
		{
			//let add it to the view port
			GameWidget->AddToViewport();
		}
		else UE_LOG(LogTemp, Warning, TEXT("ERROR PlayerPawn : couldn't load widget"));

		//Show the Cursor.
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Choose a box
/*
		BoxList --> │ 0 │ 1 │ 2 │  X
				────┼───┼───┼───┤
				  0 │ 0 │ 1 │ 2 │	
				────┼───┼───┼───┤
				  1 │ 3 │ 4 │ 5 │	
				────┼───┼───┼───┤	
				  2 │ 6 │ 7 │ 8 │	<-- Grid Panel Index
				────┴───┴───┴───┘	
				  Y
*/
void APlayerPawn::TicTacToeGame(FIntVector ButtonPressed) {
	UE_LOG(LogTemp, Warning, TEXT("Button : %d / %d"), ButtonPressed.X, ButtonPressed.Y);

	// Value : 0 = blank | 1 = cross | 2 = round
	uint8 Value = 0;
	if (!PlayerTurn) Value = 1;
	else Value = 2;

	// If blank box
	if (BoxList[ButtonPressed.X][ButtonPressed.Y] == 0) {
		BoxList[ButtonPressed.X][ButtonPressed.Y] = Value;

		// Change turn
		PlayerTurn = !PlayerTurn;
	}

	// Display / Update
	int Index = -1;
	UE_LOG(LogTemp, Warning, TEXT("> BoxList"));
	for (int y = 0; y < BoxNumber; y++) {
		for (int x = 0; x < BoxNumber; x++) {
			Index++;
			UE_LOG(LogTemp, Warning, TEXT("Box [%d][%d] {%d} = [%d]"), x, y, Index, BoxList[x][y]);
			// Appeler la fonction SetBox du Widget
			Cast<UTicTacToeWidget>(GameWidget)->UpdateGame(BoxList[x][y], Index);
		}
	}

	// See if victory for current player
	VictoryCondition(Value);
}

// Victory Condition
/*

	// Diagonals
	┌───┬───┬───┐		┌───┬───┬───┐
	│ X │   │   │		│   │   │ X │
	├───┼───┼───┤		├───┼───┼───┤
	│   │ X │   │		│   │ X │   │
	├───┼───┼───┤		├───┼───┼───┤
	│   │   │ X │		│ X │   │   │
	└───┴───┴───┘		└───┴───┴───┘

	// Vertical Lines
	┌───┬───┬───┐		┌───┬───┬───┐		┌───┬───┬───┐
	│ X │   │   │		│   │ X │   │		│   │   │ X │
	├───┼───┼───┤		├───┼───┼───┤		├───┼───┼───┤
	│ X │   │   │		│   │ X │   │		│   │   │ X │
	├───┼───┼───┤		├───┼───┼───┤		├───┼───┼───┤
	│ X │   │   │		│   │ X │   │		│   │   │ X │
	└───┴───┴───┘		└───┴───┴───┘		└───┴───┴───┘

	// Horizontal Lines
	┌───┬───┬───┐		┌───┬───┬───┐		┌───┬───┬───┐
	│ X │ X │ X │		│   │   │   │		│   │   │   │
	├───┼───┼───┤		├───┼───┼───┤		├───┼───┼───┤
	│   │   │   │		│ X │ X │ X │		│   │   │   │
	├───┼───┼───┤		├───┼───┼───┤		├───┼───┼───┤
	│   │   │   │		│   │   │   │		│ X │ X │ X │
	└───┴───┴───┘		└───┴───┴───┘		└───┴───┴───┘

*/
void APlayerPawn::VictoryCondition(uint8 IconId){
	
	// Check if all conditions are met
	bool VictoryList[BoxNumber] = { false };

	for (int i = 0; i < BoxNumber; i++) {
		if (BoxList[i][i] == IconId) {
			VictoryList[i] = true;
		}
	}
	
}