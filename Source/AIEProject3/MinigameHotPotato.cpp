// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameHotPotato.h"

#include "AIEProject3Character.h"
#include "GeometryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/TextRenderActor.h"

//called on first frame
void AMinigameHotPotato::BeginPlay()
{
	Super::BeginPlay();
	
	//check there is a valid reference to tagged text
	if(!TaggedTextBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("There was no Tagged Text blueprint attached to the minigame hot potato game mode"));
		return;
	}

	//spawns the tagged text into the world
	TaggedTextActor = GetWorld()->SpawnActor(TaggedTextBlueprint);
	
	AssignTagged();
	
}

void AMinigameHotPotato::AssignTagged()
{
	//pick a random number between 0 and 3 inclusive
	//const uint8 PlayerIndex= FMath::RandRange(0, 3);
	const uint8 PlayerIndex = 0;

	//OI
	//OI
	//OI
	//YOU BETTER CHANGE ME FROM PLAYER 0 TO A RANDOM ONE

	//Get the player from that index
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	//check the pointer is valid
	if(!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller in Minigame Hot Potato is null"));
		return;
	}

	//Set the tagged player from the controller
	TaggedPlayer = Cast<AAIEProject3Character>(PlayerController->GetPawn());

	//make sure the tagged player is not null
	if(!TaggedPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tagged Player in Minigame Hot Potato is null"));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("tagged player %s"), *TaggedPlayer->GetName())
}

void AMinigameHotPotato::Tick(float DeltaTime)
{
	//check tagged player is real
	if(!TaggedPlayer || !TaggedTextActor)
	{
		return;
	}

	const FVector PlayerLocation = TaggedPlayer->GetActorLocation();
	TaggedTextActor->SetActorLocation(PlayerLocation);
	
}


//Called when two players collide
void AMinigameHotPotato::PlayerCollision(AAIEProject3Character* Character1, AAIEProject3Character* Character2)
{
	//check that we have legit pointers
	if (!Character1 || !Character2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Null pointer for characters in MinigameHotPotato PlayerCollision"));
		return;
	}

	//if one of the characters are tagged, swap their tag status
	if (TaggedPlayer == Character1 || TaggedPlayer == Character2)
	{
		TaggedPlayer = (TaggedPlayer == Character1) ? Character2 : Character1;
	}
}

