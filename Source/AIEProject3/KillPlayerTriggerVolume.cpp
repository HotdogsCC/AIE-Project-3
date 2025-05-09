// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlayerTriggerVolume.h"
#include "GameFramework/Character.h"
#include "MinigameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MinigameDeathSwap.h"
#include "MinigameCharacterBase.h"
AKillPlayerTriggerVolume::AKillPlayerTriggerVolume()
{
    //Bind delegates
    OnActorBeginOverlap.AddDynamic(this, &AKillPlayerTriggerVolume::OnOverlapBegin);
    UE_LOG(LogTemp, Display, TEXT("%s has constructed"), *this->GetHumanReadableName());
}

void AKillPlayerTriggerVolume::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("%s has begun"), *this->GetHumanReadableName());
}

void AKillPlayerTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    //try to cast the actor in the trigger zone as a player
    if(ACharacter* Character = Cast<ACharacter>(OtherActor))
    {
        //cast the minigame mode to the base class with all my cool stuff
        if (AMinigameGameModeBase* GameMode = Cast<AMinigameGameModeBase>(UGameplayStatics::GetGameMode(this)))
        {
            //checks to see if we are running death swap
            if (AMinigameDeathSwap* DeathSwapGM = Cast<AMinigameDeathSwap>(GameMode))
            {
                //if the player has the safe guard
                AMinigameCharacterBase* AIECharacter = Cast<AMinigameCharacterBase>(Character);
                if (AIECharacter == DeathSwapGM->GetPlayerWithSafeGuard())
                {
                    //dont do anything, because they shouldnt die
                    return;
                }
            }

            //get the player controller
            if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
            {
                // tell the game mode player x died
                GameMode->DeclareDeadPlayer(PlayerController->GetLocalPlayer()->GetControllerId());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Mode not found!"));
        }

        UE_LOG(LogTemp, Display, TEXT("%s just killed %s"), *this->GetName(), *Character->GetName());
        Character->Destroy();
    }
}