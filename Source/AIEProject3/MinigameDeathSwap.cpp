// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameDeathSwap.h"
#include "AIEProject3Character.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"

void AMinigameDeathSwap::BeginPlay()
{
    Super::BeginPlay();

    InitialNumOfPlayers = GetPlayersAlive();
}

void AMinigameDeathSwap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TimeLimit == 0)
    {
        SwapPlayers(InitialNumOfPlayers);
        TimeLimit = InitTimeLimit;
    }

    //someone just died
    if(InitialNumOfPlayers != GetPlayersAlive())
    {
        UGameplayStatics::OpenLevel(this, FName("ProgrammingPrototyping"));
    }
    
}

void AMinigameDeathSwap::SwapPlayers(uint8 NumOfPlayers)
{
    FVector TempPosition;
    switch (NumOfPlayers)
    {
    case 2:
        
        //temp pos for where player 1 is
        TempPosition = Players[0]->GetActorLocation();
        //moves player 1 to player 2
        Players[0]->SetActorLocation(Players[1]->GetActorLocation());
        //moves player 2 to whewre player 1 was
        Players[1]->SetActorLocation(TempPosition);
        break;

    case 3:
        //temp pos for where player 1 is
        TempPosition = Players[0]->GetActorLocation();

        //moves player 1 to player 2
        Players[0]->SetActorLocation(Players[1]->GetActorLocation());
        //moves player 2 to player 3
        Players[1]->SetActorLocation(Players[2]->GetActorLocation());
        //moves player 3 to player 1
        Players[2]->SetActorLocation(TempPosition);
        break;

    case 4:
        //temp pos for where player 1 is
        TempPosition = Players[0]->GetActorLocation();
        //moves player 1 to player 2
        Players[0]->SetActorLocation(Players[1]->GetActorLocation());
        //moves player 2 to whewre player 1 was
        Players[1]->SetActorLocation(TempPosition);

        //temp pos for where player 3 is
        TempPosition = Players[2]->GetActorLocation();
        //moves player 3 to player 4
        Players[2]->SetActorLocation(Players[3]->GetActorLocation());
        //moves player 2 to whewre player 1 was
        Players[3]->SetActorLocation(TempPosition);

        break;

        default:
        break;

    }


}

void AMinigameDeathSwap::DeclarePlayer(AAIEProject3Character* PlayerPointer)
{
    AMinigameGameModeBase::DeclarePlayer(PlayerPointer);

    UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    if(!GameInstance->IsPlayerAlive(PlayerPointer))
    {
        PlayerPointer->Destroy();
    }
}