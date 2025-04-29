// Fill out your copyright notice in the Description page of Project Settings.


#include "PipeTravel.h"
#include "Engine/TargetPoint.h"

APipeTravel::APipeTravel()
{
	//make it so the actor actually ticks
	PrimaryActorTick.bCanEverTick = true;
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

		//checks if player is at the target location
		if (DirectionVector.Length() >= DistanceToTarget)
		{
			//set player location to the target
			Players[i]->SetActorLocation(TargetLocation);

			//increment target index
			TargetIndexs[i]++;

			//if there are no more targets to travel to, eject
			if (TargetIndexs[i] >= Targets.Num())
			{
				DirectionVector.Normalize();
				Eject(Players[i], DirectionVector, EjectForce); //blueprint function
				Players[i] = nullptr;
				TargetIndexs[i] = -1;
			}
		}
		else
		{
			//set player location to move closer to target
			FVector NewPosition = CurrentLocation + DirectionVector;
			Players[i]->SetActorLocation(NewPosition);
		}
	}
	
}
