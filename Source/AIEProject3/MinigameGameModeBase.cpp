// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameGameModeBase.h"

AMinigameGameModeBase::AMinigameGameModeBase()
{
PrimaryActorTick.bStartWithTickEnabled = true;
PrimaryActorTick.bCanEverTick = true;
}



// Called every frame
void AMinigameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //UE_LOG(LogTemp, Display, TEXT("Tick Tock"));
}

void AMinigameGameModeBase::DeclareDeadPlayer(uint8 PlayerNum)
{
    UE_LOG(LogTemp, Display, TEXT("Player %i died"), PlayerNum);
}