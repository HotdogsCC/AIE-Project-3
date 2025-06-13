// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ppltasks.h>

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AIEPROJECT3_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	uint8 GetPlayerWins(uint8 PlayerNum) const;

	UFUNCTION(BlueprintCallable)
	void AddPlayerWin(uint8 PlayerNum);

	UFUNCTION(BlueprintCallable)
	void PrintLevelArray();

	UFUNCTION(BlueprintCallable)
	void LoadRandomMinigame();

	void LoadMinigameSelection();

	UFUNCTION(BlueprintCallable)
	void LoadWinScreen();

	UFUNCTION(BlueprintCallable)
	void PlayerJoined(uint8 PlayerNum);

	UFUNCTION(BlueprintCallable)
	void RemovePlayersFromSession();

	UFUNCTION(BlueprintCallable)
	void EnableSplitscreen();
	
	UFUNCTION(BlueprintCallable)
	void DisableSplitscreen();

	bool IsPlayerAlive(uint8 PlayerNumber);

	bool IsPlayerAlive(class AMinigameCharacterBase* PlayerPointer);

	UFUNCTION(BlueprintCallable)
	void SetIsPlayerAlive(uint8 PlayerNumber, bool status);

	UFUNCTION(BlueprintCallable)
	void SetAllPlayersAlive();

	UFUNCTION(BlueprintCallable)
	FName GetNameOfLevelToLoad();

	UFUNCTION(BlueprintCallable)
	void SetNameOfLevelToLoad(FName name);

	UFUNCTION(BlueprintCallable)
	uint8 GetPlayerWhoWonIndex();

	UFUNCTION(BlueprintCallable)
	void PlayMinigameSelectionMusic(UObject* WorldContextObject, USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	void StopMinigameMusic();


private:
	UPROPERTY()
	TArray<FString> LevelNames =
	{
		"ProgrammingPrototyping"
	};
	
	UPROPERTY()
	uint8 PlayerWins[4] = {0, 0, 0, 0};
	bool PlayersJoined[4] = {false, false, false, false};
	//Becomes true when all players have joined
	bool GameReadyToStart = false;
	bool bIsPlayersAlive[4] = {true, true, true, true};

	//set by the minigame selection
	FName NameOfLevelToLoad;

	//stores who won
	uint8 PlayerWhoWonIndex = 10;

	UPROPERTY()
	//stores the minigame music currently playing
	UAudioComponent* MinigameMusic = nullptr;
};
