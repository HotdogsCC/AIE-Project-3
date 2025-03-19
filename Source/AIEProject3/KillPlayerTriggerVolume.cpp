// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlayerTriggerVolume.h"
#include "GameFramework/Character.h"
#include "MinigameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
AKillPlayerTriggerVolume::AKillPlayerTriggerVolume()
{
    //Bind delegates
    OnActorBeginOverlap.AddDynamic(this, &AKillPlayerTriggerVolume::OnOverlapBegin);
}

void AKillPlayerTriggerVolume::BeginPlay()
{
    Super::BeginPlay();

}

void AKillPlayerTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    //try to cast the actor in the trigger zone as a player
    ACharacter* Character = Cast<ACharacter>(OtherActor);

    if(Character)
    {
        //cast the minigame mode to the base class with all my cool stuff
        AMinigameGameModeBase* GameMode = Cast<AMinigameGameModeBase>(UGameplayStatics::GetGameMode(this));
        if (GameMode)
        {
            //get the player controller
            APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
            if(PlayerController)
            {
                // tell the game mode player x died
                GameMode->DeclareDeadPlayer(PlayerController->GetLocalPlayer()->GetControllerId());
            }
            
            
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Mode not found!"));
        }

        UE_LOG(LogTemp, Display, TEXT("Killing %s"), *Character->GetName());
        Character->Destroy();
    }
}