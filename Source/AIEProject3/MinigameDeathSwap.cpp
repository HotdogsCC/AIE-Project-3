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
    
    TArray<AAIEProject3Character*> PlayersThatAreAlive;
    UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    for(int i = 0; i < 4; i++)
    {
        if(GameInstance->IsPlayerAlive(i))
        {
            PlayersThatAreAlive.Add(Players[i]);
        }
    }
        
    switch (NumOfPlayers)
    {
    case 2:
        
        //temp pos for where player 1 is
        TempPosition = PlayersThatAreAlive[0]->GetActorLocation();
        //moves player 1 to player 2
        PlayersThatAreAlive[0]->SetActorLocation(PlayersThatAreAlive[1]->GetActorLocation());
        //moves player 2 to whewre player 1 was
        PlayersThatAreAlive[1]->SetActorLocation(TempPosition);
        break;

    case 3:
        //temp pos for where player 1 is
        TempPosition = PlayersThatAreAlive[0]->GetActorLocation();

        //moves player 1 to player 2
        PlayersThatAreAlive[0]->SetActorLocation(PlayersThatAreAlive[1]->GetActorLocation());
        //moves player 2 to player 3
        PlayersThatAreAlive[1]->SetActorLocation(PlayersThatAreAlive[2]->GetActorLocation());
        //moves player 3 to player 1
        PlayersThatAreAlive[2]->SetActorLocation(TempPosition);
        break;

    case 4:
        //temp pos for where player 1 is
        TempPosition = PlayersThatAreAlive[0]->GetActorLocation();
        //moves player 1 to player 2
        PlayersThatAreAlive[0]->SetActorLocation(PlayersThatAreAlive[1]->GetActorLocation());
        //moves player 2 to whewre player 1 was
        PlayersThatAreAlive[1]->SetActorLocation(TempPosition);

        //temp pos for where player 3 is
        TempPosition = PlayersThatAreAlive[2]->GetActorLocation();
        //moves player 3 to player 4
        PlayersThatAreAlive[2]->SetActorLocation(PlayersThatAreAlive[3]->GetActorLocation());
        //moves player 2 to whewre player 1 was
        PlayersThatAreAlive[3]->SetActorLocation(TempPosition);

        break;

        default:
        break;

    }


}

void AMinigameDeathSwap::DeclarePlayer(AAIEProject3Character* PlayerPointer)
{
    AMinigameGameModeBase::DeclarePlayer(PlayerPointer);

    //kills the player if he is meant to be dead
    UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
    if(!GameInstance->IsPlayerAlive(PlayerPointer))
    {
        CreateDeadWidget(PlayerPointer);
        PlayerPointer->Destroy();
    }
}