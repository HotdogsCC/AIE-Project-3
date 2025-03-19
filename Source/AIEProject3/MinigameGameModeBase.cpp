// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameGameModeBase.h"

#include "PopUpUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


AMinigameGameModeBase::AMinigameGameModeBase()
{
PrimaryActorTick.bStartWithTickEnabled = true;
PrimaryActorTick.bCanEverTick = true;
}



// Called every frame
void AMinigameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(bTimerOn)
    {
    	TimeLimit -= DeltaTime;
    	if(TimeLimit <= 0)
    	{
    		UE_LOG(LogTemp, Display, TEXT("Ran out of time, Game Over!"));
    	}
	    PopUpWidgetInstance->SetText(FString::FromInt(FMath::CeilToInt(TimeLimit)));
    }
}

void AMinigameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if(EndMode == FEndMode::TIMER)
	{
		if(!TimerWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("The Timer Widget is not set."))
			return;
		}
		
		//Makes the spawn timer widget
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), TimerWidget); 
		if(!WidgetInstance) return;
	
		PopUpWidgetInstance = Cast<UPopUpUserWidget>(WidgetInstance);
		if(!PopUpWidgetInstance) return;

		PopUpWidgetInstance->SetText(TEXT("Timer"));

		PopUpWidgetInstance->AddToViewport();
		bTimerOn = true;
	}
}

void AMinigameGameModeBase::DeclareDeadPlayer(uint8 PlayerNum)
{
    UE_LOG(LogTemp, Display, TEXT("Player %i died"), PlayerNum);
	bIsPlayersAlive[PlayerNum] = false;

	if(EndMode == FEndMode::LASTPLAYER)
	{
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

		//If only one player is left alive
		if(PlayersAlive == 1)
		{
			//they won!
			PlayerWon(PlayerIndexAlive);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You killed a player, but the game mode isn't set to 'LASTPLAYER'. Was this a mistake?"));
	}
	
}

void AMinigameGameModeBase::PlayerWon(uint8 PlayerNum)
{
	UE_LOG(LogTemp, Display, TEXT("Player %i won!"), PlayerNum);
}
