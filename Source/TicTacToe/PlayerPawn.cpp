// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

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
		GameWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

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
	
	UE_LOG(LogTemp, Warning, TEXT("> BoxList"));
	for (int i = 0; i < BoxNumber; i++) {
		for (int j = 0; j < BoxNumber; j++) {
			UE_LOG(LogTemp, Warning, TEXT("Box [%d][%d] = [%d]"), i, j, BoxList[i][j]);
			// Appeller la fonction SetBox du Widget
		}
	}
	
}