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
    ACharacter* Character = Cast<ACharacter>(OtherActor);

    if(Character)
    {
        AMinigameGameModeBase* GameMode = Cast<AMinigameGameModeBase>(UGameplayStatics::GetGameMode(this));
        if (GameMode)
        {
            // Call the function in the Game Mode
            GameMode->DeclareDeadPlayer(255);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Game Mode not found!"));
        }

        UE_LOG(LogTemp, Display, TEXT("Killing %s"), *Character->GetName());
        Character->Destroy();
    }
}