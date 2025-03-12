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
private:
	UPROPERTY()
	TArray<FString> LevelNames =
	{
		"Example 1",
		"Example 2",
		"Example 3",
		"Example 4",
		"Example 5",
		"Example 6",
		"Example 7",
		"Example 8",
		"Example 9",
		"Example 10"
	};
	
	UPROPERTY()
	uint8 PlayerWins[4] = {0, 0, 0, 0};
	
};
