// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

#include "Kismet/GameplayStatics.h"

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
	if(RandLevel.Equals(TEXT("ThirdPersonMap")))
	{
		UGameplayStatics::OpenLevel(this, FName(*RandLevel));
	}
}

