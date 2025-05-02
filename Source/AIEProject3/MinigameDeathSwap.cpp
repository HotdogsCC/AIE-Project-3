// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameDeathSwap.h"
#include "AIEProject3Character.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"
#include "Engine/TextRenderActor.h"

void AMinigameDeathSwap::BeginPlay()
{
    Super::BeginPlay();

    InitialNumOfPlayers = GetPlayersAlive();

    if (!SafeGuardTextBlueprint)
    {
        UE_LOG(LogTemp, Warning, TEXT("There was no safe guard Text blueprint attached to the minigame death swap game mode"));
        return;
    }

    //Spawn the text actor
    SafeGuardTextActor = GetWorld()->SpawnActor(SafeGuardTextBlueprint);

    //make it go away
    FVector MiddleOfNowhere(0.0f, 0.0f, -99999.0f);
    SafeGuardTextActor->SetActorLocation(MiddleOfNowhere);
}

void AMinigameDeathSwap::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TimeLimit == 0)
    {
        SwapPlayers(InitialNumOfPlayers);
        TimeLimit = InitTimeLimit;
    }

    if (SafeGuardedPlayer && SafeGuardTextActor)
    {
        ProcessSafeGuard();
    }

    //someone just died
    if(InitialNumOfPlayers != GetPlayersAlive())
    {
        //if there is only 1 player left, the game is over
        if(GetPlayersAlive() == 1)
        {
            //figures out which player is alive
            UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
            for(int i = 0; i < 4; i++)
            {
                if(GameInstance->IsPlayerAlive(i))
                {
                    //gives that player a win
                    GameInstance->AddPlayerWin(i);
                    UGameplayStatics::OpenLevel(this, FName("MinigameSelection"));
                    return;
                }
            }
            
        }
        //how many players are alive after someone died
        switch (GetPlayersAlive())
        {
        case 3:
            UGameplayStatics::OpenLevel(this, FName("WG_2Death_Swap_MIni-game"));
            break;
        case 2:
            UGameplayStatics::OpenLevel(this, FName("WG_3Death_Swap_MIni-game"));
            break;
        case 1:
            UGameplayStatics::OpenLevel(this, FName("MinigameSelection"));
        default:
            UGameplayStatics::OpenLevel(this, FName("TitleScreen"));
            break;
        }
        
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

void AMinigameDeathSwap::ProcessSafeGuard()
{
    const FVector PlayerLocation = SafeGuardedPlayer->GetActorLocation();
    SafeGuardTextActor->SetActorLocation(SafeGuardedPlayer->GetActorLocation());
}

void AMinigameDeathSwap::SetPlayerWithSafeGuard(AAIEProject3Character* PlayerPointer)
{
    if (PlayerPointer)
    {
        SafeGuardedPlayer = PlayerPointer;
    }
    
}