// Fill out your copyright notice in the Description page of Project Settings.


#include "PipeTravel.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "MinigameHotPotato.h"
#include "GameFramework/Character.h"

APipeTravel::APipeTravel()
{
	//make it so the actor actually ticks
	PrimaryActorTick.bCanEverTick = true;
}

void APipeTravel::BeginPlay()
{
	Super::BeginPlay();

	//get game mode
	AGameModeBase* BaseGameMode = UGameplayStatics::GetGameMode(this);
	if (!BaseGameMode)
	{
		return;
	}

	//cast to hot potato game mode
	AMinigameHotPotato* GameMode = Cast<AMinigameHotPotato>(BaseGameMode);
	if (!GameMode)
	{
		return;
	}

	//tell the hot potato game mode that this pipe exists, so it can delete dead players
	GameMode->AddPipe(this);
}

//every frame
void APipeTravel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Targets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pipe %s requrires at least 1 target"), *GetName());
		return;
	}

	//tries to travel, fails if null
	Travel(DeltaTime);
}

//Called by blueprint, player input is already disabled, lets this class know to start schmovin the player
void APipeTravel::StartTravelling(AActor* InCharacter)
{
	//check that the player is not already in the pipe
	for(int32 i = 0; i < 4; i++)
	{
		//is the character already in the list of characters to travel?
		if(Players[i] == InCharacter)
		{
			//do nothing more
			return;
		}
	}
	
	//loops through the array of player pointers and finds first slot available
	for(int32 i = 0; i < 4; i++)
	{
		if(Players[i] == nullptr)
		{
			Players[i] = InCharacter;
			TargetIndexs[i] = 0;
			break;
		}
	}
	
}

//runs every tick
void APipeTravel::Travel(float DeltaTime)
{
	//loops through each player
	for(int32 i = 0; i < 4; i++)
	{
		//check if there is a valid pointer
		if(Players[i] == nullptr)
		{
			//move on if null
			continue;
		}

		//temp fix that teleports player straight to the first target point
		if (TargetIndexs[i] == 0)
		{
			//play pipe enter sound
			UGameplayStatics::PlaySound2D(this, EnterSound);
			
			ATargetPoint* TargetPoint = Targets[TargetIndexs[i]];
			FVector TargetLocation = TargetPoint->GetActorLocation();
			Players[i]->SetActorLocation(TargetLocation);
			TargetIndexs[i]++;
			return;
		}

		//stores where the player currently is
		FVector CurrentLocation = Players[i]->GetActorLocation();

		//stores where the player is meant to be going
		ATargetPoint* TargetPoint = Targets[TargetIndexs[i]];
		FVector TargetLocation = TargetPoint->GetActorLocation();

		//gets direction to the location
		FVector DirectionVector = TargetLocation - CurrentLocation;

		//gets distance from the location
		float DistanceToTarget = DirectionVector.Length();;
	
		//scales by the distance it should travel
		DirectionVector.Normalize();
		DirectionVector = DirectionVector * TravelSpeed * DeltaTime;

		//get game mode
		AGameModeBase* BaseGameMode = UGameplayStatics::GetGameMode(this);
		if (!BaseGameMode)
		{
			return;
		}

		//cast to hot potato game mode
		AMinigameHotPotato* GameMode = Cast<AMinigameHotPotato>(BaseGameMode);
		AActor* PlayerHighlighter = nullptr;

		//if the game mode exists
		if (GameMode)
		{
			//get the player highlighter
			ACharacter* InChar = Cast<ACharacter>(Players[i]);
			AController* Controller = InChar->GetController();
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			int32 playerIndex = PlayerController->GetLocalPlayer()->GetControllerId();

			PlayerHighlighter = GameMode->GetPlayerHighlighter(playerIndex);
		}

		

		//checks if player is at the target location
		if (DirectionVector.Length() >= DistanceToTarget)
		{
			//set player location to the target
			Players[i]->SetActorLocation(TargetLocation);

			if(PlayerHighlighter)
			{
				//move highlighter to new pos
				PlayerHighlighter->SetActorLocation(Players[i]->GetActorLocation());
			}
			

			//increment target index
			TargetIndexs[i]++;

			//if there are no more targets to travel to, eject
			if (TargetIndexs[i] >= Targets.Num())
			{

				if(PlayerHighlighter)
				{
					FVector MiddleOfNowhere(9999, 9999, 9999);
					PlayerHighlighter->SetActorLocation(MiddleOfNowhere);
				}
				

				DirectionVector.Normalize();
				Eject(Players[i], DirectionVector, EjectForce); //blueprint function
				Players[i] = nullptr;
				TargetIndexs[i] = -1;

				//play pipe exit sound
				UGameplayStatics::PlaySound2D(this, ExitSound);
			}
		}
		else
		{
			//set player location to move closer to target
			FVector NewPosition = CurrentLocation + DirectionVector;
			Players[i]->SetActorLocation(NewPosition);

			if(PlayerHighlighter)
			{
				//move highlighter to new pos
				PlayerHighlighter->SetActorLocation(Players[i]->GetActorLocation());
			}
			
		}

		
	}
	
}

void APipeTravel::SetPlayerDead(AActor* InCharacter)
{
	UE_LOG(LogTemp, Display, TEXT("I, the pipe, have run the Set Player Dead function"));
	for (int32 i = 0; i < 4; i++)
	{
		if (InCharacter == Players[i])
		{
			UE_LOG(LogTemp, Display, TEXT("I found a player which should be dead!!!!"));

			//get game mode
			AGameModeBase* BaseGameMode = UGameplayStatics::GetGameMode(this);
			if (!BaseGameMode)
			{
				return;
			}

			//cast to hot potato game mode
			AMinigameHotPotato* GameMode = Cast<AMinigameHotPotato>(BaseGameMode);
			if (GameMode)
			{
				//get the player highlighter
				ACharacter* InChar = Cast<ACharacter>(Players[i]);
				AController* Controller = InChar->GetController();
				APlayerController* PlayerController = Cast<APlayerController>(Controller);
				int32 playerIndex = PlayerController->GetLocalPlayer()->GetControllerId();

				AActor* PlayerHighlighter = GameMode->GetPlayerHighlighter(playerIndex);

				//move the highlighter to the middle of nowhere positiion
				FVector MiddleOfNowhere(9999, 9999, 9999);
				PlayerHighlighter->SetActorLocation(MiddleOfNowhere);
			}

			Players[i] = nullptr;
			return;
		}
	}
}