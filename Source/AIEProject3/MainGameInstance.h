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

	UFUNCTION(BlueprintCallable)
	void PlayerJoined(uint8 PlayerNum);

	UFUNCTION(BlueprintCallable)
	void EnableSplitscreen();
	
	UFUNCTION(BlueprintCallable)
	void DisableSplitscreen();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerAlive(uint8 PlayerNumber);

	UFUNCTION(BlueprintCallable)
	void SetIsPlayerAlive(uint8 PlayerNumber, bool status);


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
};
