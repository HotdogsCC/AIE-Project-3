// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameGameModeBase.h"

AMinigameGameModeBase::AMinigameGameModeBase()
{
PrimaryActorTick.bStartWithTickEnabled = true;
PrimaryActorTick.bCanEverTick = true;
}



// Called every frame
void AMinigameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //UE_LOG(LogTemp, Display, TEXT("Tick Tock"));
}

void AMinigameGameModeBase::DeclareDeadPlayer(uint8 PlayerNum)
{
    UE_LOG(LogTemp, Display, TEXT("Player %i died"), PlayerNum);
	bIsPlayersAlive[PlayerNum] = false;

	//checks to see which player is alive
	uint8 PlayersAlive = 0;
	uint8 PlayerIndexAlive = -1;
	for(uint8 i = 0; i < 4; i++)
	{
		//if the player is alive
		if(bIsPlayersAlive[i])
		{
			PlayersAlive++;
			PlayerIndexAlive = i;
		}
	}

	if(PlayersAlive == 1)
	{
		PlayerWon(PlayerIndexAlive);
	}
}

void AMinigameGameModeBase::PlayerWon(uint8 PlayerNum)
{
	UE_LOG(LogTemp, Display, TEXT("Player %i won!"), PlayerNum);
}
