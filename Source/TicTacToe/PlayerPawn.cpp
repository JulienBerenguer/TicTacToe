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
				  2 │ 6 │ 7 │ 8 │	<-- Grid Panel Index ( = 1*X + BOX_NUMBER*Y )
				────┴───┴───┴───┘	
				  Y
*/
void APlayerPawn::TicTacToeGame(FIntVector ButtonPressed) {
	UE_LOG(LogTemp, Warning, TEXT("Button : %d / %d"), ButtonPressed.X, ButtonPressed.Y);

	// If neither player win => can continue to play
	if (!GameOver) 
	{
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
		for (int y = 0; y < BOX_NUMBER; y++) {
			for (int x = 0; x < BOX_NUMBER; x++) {
				Index++;
				UE_LOG(LogTemp, Warning, TEXT("Box [%d][%d] {%d} = [%d]"), x, y, Index, BoxList[x][y]);
				// Appeler la fonction SetBox du Widget
				Cast<UTicTacToeWidget>(GameWidget)->UpdateGame(BoxList[x][y], Index);
			}
		}

		// See if victory for current player
		GameOver = VictoryCondition(Value);
	}
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
bool APlayerPawn::VictoryCondition(uint8 PlayerId){
	
	// Diagonal 1 (only if BOX_NUMBER = 3)
	for (int i = 0; i < BOX_NUMBER; i++) {
		if (BoxList[i][i] == PlayerId) {
			VictoryList[i] = true;
		}
	}
	if (VictoryScreen(PlayerId)) return true;


	// Diagonal 2 (only if BOX_NUMBER = 3)
	for (int i = 0; i < BOX_NUMBER; i++) {
		if (BoxList[i][(BOX_NUMBER-1)-i] == PlayerId) {
			VictoryList[i] = true;
		}
	}
	if (VictoryScreen(PlayerId)) return true;


	// Vertical lines
	for (int x = 0; x < BOX_NUMBER; x++) {		// Change column
		for (int y = 0; y < BOX_NUMBER; y++) {	// All boxes of actual column
			if (BoxList[x][y] == PlayerId) {
				VictoryList[y] = true;
			}
		}
		if (VictoryScreen(PlayerId)) return true;
	}

	// Horizontal lines
	for (int y = 0; y < BOX_NUMBER; y++) {		// Change line
		for (int x = 0; x < BOX_NUMBER; x++) {	// All boxes of actual line
			if (BoxList[x][y] == PlayerId) {
				VictoryList[x] = true;
			}
		}
		if (VictoryScreen(PlayerId)) return true;
	}

	// If all lines are filled, but no one win
	bool Loose = true;
	for (int y = 0; y < BOX_NUMBER; y++) {
		for (int x = 0; x < BOX_NUMBER; x++) {
			if (BoxList[x][y] == 0) {	// If blank
				Loose = false;
			}
		}
	}
	if (Loose) {
		Cast<UTicTacToeWidget>(GameWidget)->Defeat();
		return true;
	}

	return false;
	
}

// If condition valid, then player win
bool APlayerPawn::VictoryScreen(uint8 PlayerId) {

	bool IsWinner = true;

	// If Victory List contain a single false, then the player don't win
	for (int i = 0; i < BOX_NUMBER; i++) {
		if (VictoryList[i] != true) {
			IsWinner = false;
			break;
		}
	}

	// Actual player win
	if (IsWinner) {
		Cast<UTicTacToeWidget>(GameWidget)->Victory(PlayerId);
	}
	// Game continue
	else {
		// Reset VictoryList
		for (int i = 0; i < BOX_NUMBER; i++) {
			VictoryList[i] = false;
		}
	}

	return IsWinner;
}