// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameGameModeBase.h"

#include "PopUpUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MinigameCharacterBase.h"
#include "MainGameInstance.h"

AMinigameGameModeBase::AMinigameGameModeBase()
{
PrimaryActorTick.bStartWithTickEnabled = true;
PrimaryActorTick.bCanEverTick = true;
}



// Called every frame
void AMinigameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTimer(DeltaTime);
}

void AMinigameGameModeBase::UpdateTimer(float DeltaTime)
{
	if(bTimerOn)
	{
		TimeLimit -= DeltaTime;
		if(TimeLimit <= 0)
		{
			TimeLimit = 0;
		}
		if(PopUpWidgetInstance)
		{
			PopUpWidgetInstance->SetText(FString::FromInt(FMath::CeilToInt(TimeLimit)));
		}
		
	}
}


void AMinigameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitTimeLimit = TimeLimit;

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

int AMinigameGameModeBase::GetPlayersAlive()
{
	uint8 PlayersAlive = 0;
	for(uint8 i = 0; i < 4; i++)
	{
		//checks if player i is alive
		UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
		if(GameInstance->IsPlayerAlive(i))
		{
			PlayersAlive++;
		}
	}

	return PlayersAlive;
}

void AMinigameGameModeBase::DeclareDeadPlayer(uint8 PlayerNum)
{
    UE_LOG(LogTemp, Display, TEXT("Player %i died"), PlayerNum);

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, PlayerNum);
	if (Character)
	{
		DisplayDeadWidget(Character);
		//spawn blood
		if (Blood)
		{
			const FVector Location = Character->GetActorLocation();
			GetWorld()->SpawnActor(Blood, &Location);
		}
	}

	
	
	
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	GameInstance->SetIsPlayerAlive(PlayerNum, false);

	if(EndMode == FEndMode::LASTPLAYER)
	{
		//checks to see which player is alive
		uint8 PlayersAlive = 0;
		uint8 PlayerIndexAlive = -1;
		for(uint8 i = 0; i < 4; i++)
		{
			//if the player is alive
			if(GameInstance->IsPlayerAlive(i))
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

	if (EndMode == FEndMode::TUTORIAL)
	{
		//checks to see which player is alive
		uint8 PlayersAlive = 0;
		uint8 PlayerIndexAlive = -1;
		for (uint8 i = 0; i < 4; i++)
		{
			//if the player is alive
			if (GameInstance->IsPlayerAlive(i))
			{
				PlayersAlive++;
				PlayerIndexAlive = i;
			}
		}

		//If no one is alive
		if (PlayersAlive == 0)
		{
			//tutorial end
			UGameplayStatics::OpenLevel(this, "MinigameSelection");
		}
	}
	
}

void AMinigameGameModeBase::PlayerWon(uint8 PlayerNum)
{
	UE_LOG(LogTemp, Display, TEXT("Player %i won!"), PlayerNum);

	//increments the amount of player wins
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->AddPlayerWin(PlayerNum);
		//go to win screen
		GameInstance->LoadWinScreen();
	}

	

	//GameInstance->LoadMinigameSelection();

}

//When two players collide, can be overrided for specific functionality in minigames
void AMinigameGameModeBase::PlayerCollision(AMinigameCharacterBase* Character1, AMinigameCharacterBase* Character2)
{
	if (!Character1 || !Character2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Null pointer for characters in MinigameGameModeBase PlayerCollision"));
		return;
	}
}

void AMinigameGameModeBase::DeclarePlayer(AMinigameCharacterBase* PlayerPointer)
{
	if(!PlayerPointer)
	{
		return;
	}

	AController* MyPlayer = PlayerPointer->GetController();
	if(!MyPlayer)
	{
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(MyPlayer);
	if(!PlayerController)
	{
		return;
	}
	Players[PlayerController->GetLocalPlayer()->GetControllerId()] = PlayerPointer;

	UE_LOG(LogTemp, Display, TEXT("amazing"));
}

void AMinigameGameModeBase::DisplayDeadWidget(ACharacter* Character)
{
	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (PlayerController)
		{
			if (DeathWidget)
			{
				UUserWidget* WidgetInstance = CreateWidget(PlayerController, DeathWidget);
				if (WidgetInstance)
				{
					WidgetInstance->AddToPlayerScreen();
				}
			}
			
		}
	}
	

	
}