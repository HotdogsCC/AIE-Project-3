// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinigameGameModeBase.generated.h"
class AAIEProject3Character; // cheeky forward declare

/**
 * 
 */
UENUM(BlueprintType)
enum class FEndMode : uint8 {
	LASTPLAYER = 0 UMETA(DisplayName = "LASTPLAYER"),
	TIMER = 1 UMETA(DisplayName = "TIMER"),
	OBJECTIVE = 2 UMETA(DisplayName = "OBJECTIVE"),
	OTHER = 255 UMETA(DisplayName = "OTHER")
};

UCLASS()
class AIEPROJECT3_API AMinigameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//Constructor
	AMinigameGameModeBase();
	
	//Called for a player win
	void PlayerWon(uint8 PlayerNum);

	//Called for the Last Player gamemode
	void DeclareDeadPlayer(uint8 PlayerNum);

	//displays a widget saying the player died
	void DisplayDeadWidget(ACharacter* Player);

	///Called when two players collide
	virtual void PlayerCollision(AAIEProject3Character* Character1, AAIEProject3Character* Character2);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Timer")
	TSubclassOf<UUserWidget> TimerWidget;
	UPROPERTY(EditDefaultsOnly, Category="Timer")
	float TimeLimit;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> DeathWidget;

	int GetPlayersAlive();

	virtual void DeclarePlayer(AAIEProject3Character* PlayerPointer);

	//blood
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> Blood;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateTimer(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category="Minigame Config")
	FEndMode EndMode;
	class AAIEProject3Character* Players[4];
	float InitTimeLimit = -1.0f;

private:
	
	class UPopUpUserWidget* PopUpWidgetInstance;
	bool bTimerOn = false;


	
};
