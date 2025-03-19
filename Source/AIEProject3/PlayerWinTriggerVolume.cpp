// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWinTriggerVolume.h"
#include "GameFramework/Character.h"
#include "MinigameGameModeBase.h"
#include "Kismet/GameplayStatics.h"

APlayerWinTriggerVolume::APlayerWinTriggerVolume()
{
	//Bind delegates
	OnActorBeginOverlap.AddDynamic(this, &APlayerWinTriggerVolume::OnOverlapBegin);
}

void APlayerWinTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerWinTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	//try to cast the actor in the trigger zone as a player
	if(ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		//cast the minigame mode to the base class with all my cool stuff
		if (AMinigameGameModeBase* GameMode = Cast<AMinigameGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			//get the player controller
			if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
			{
				// tell the game mode player x died
				GameMode->PlayerWon(PlayerController->GetLocalPlayer()->GetControllerId());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Mode not found!"));
		}
	}
}
