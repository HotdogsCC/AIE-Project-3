// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "MinigameCharacterBase.h"

//Returns the number of wins for a given player
uint8 UMainGameInstance::GetPlayerWins(const uint8 PlayerNum) const
{
	return PlayerWins[PlayerNum];
}

//Increments the amount of wins for a given player
void UMainGameInstance::AddPlayerWin(const uint8 PlayerNum)
{
	PlayerWins[PlayerNum]++;
}

//Prints the name of each level stored in the Game Instance
void UMainGameInstance::PrintLevelArray()
{
	for(const FString& LevelName : LevelNames)
	{
		UE_LOG(LogTemp, Display, TEXT("Level: %s"), *LevelName);
	}
}

//Loads a random minigame
void UMainGameInstance::LoadRandomMinigame()
{
	if(LevelNames.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ran out of minigames"))
		return;
	}
	int RandIndex = FMath::RandRange(0, LevelNames.Num() - 1);
	FString RandLevel = LevelNames[RandIndex];
	LevelNames.RemoveAt(RandIndex);

	//LOAD LEVEL HERE
	UE_LOG(LogTemp, Display, TEXT("Load %s"), *RandLevel);
	UGameplayStatics::OpenLevel(this, FName(*RandLevel));
}

//Runs when a player joins from the lobby
void UMainGameInstance::PlayerJoined(uint8 PlayerNum)
{
	PlayersJoined[PlayerNum] = true;
	//assumes game can start until proven otherwise
	bool CanStart = true;
	for(uint8 i = 0; i < 4; i++)
	{
		if(PlayersJoined[i] == false)
		{
			CanStart = false;
			break;
		}
	}
	//GameReadyToStart is a member variable, CanStart is a temp variable
	GameReadyToStart = CanStart;

	if(GameReadyToStart)
	{
		LoadMinigameSelection();
		//LoadRandomMinigame();
	}
	
}

void UMainGameInstance::EnableSplitscreen()
{
	if (!GetWorld()->GetGameViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to enable splitscreen: GameViewport not available"));
		return;
	}
    
	// Enable splitscreen
	GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(false);
}

void UMainGameInstance::DisableSplitscreen()
{
	if (!GetWorld()->GetGameViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to disable splitscreen: GameViewport not available"));
		return;
	}
    
	// Enable splitscreen
	GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(true);
}


bool UMainGameInstance::IsPlayerAlive(uint8 PlayerNumber)
{
	return bIsPlayersAlive[PlayerNumber];
}

bool UMainGameInstance::IsPlayerAlive(AMinigameCharacterBase* PlayerPointer)
{
	AController* MyPlayer = PlayerPointer->GetController();
	APlayerController* PlayerController = Cast<APlayerController>(MyPlayer);
	return IsPlayerAlive(PlayerController->GetLocalPlayer()->GetControllerId());
}

void UMainGameInstance::SetIsPlayerAlive(uint8 PlayerNumber, bool status)
{
	bIsPlayersAlive[PlayerNumber] = status;
}

void UMainGameInstance::LoadMinigameSelection()
{
	UGameplayStatics::OpenLevel(this, FName("MinigameSelection"));
}