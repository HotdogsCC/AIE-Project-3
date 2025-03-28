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

	//If the player should be travelling, travel!
	if (bIsTravelling)
	{
		Travel(DeltaTime);
	}
}

//Called by blueprint, player input is already disabled, lets this class know to start schmovin the player
void APipeTravel::StartTravelling(AActor* InCharacter)
{
	bIsTravelling = true;
	Player = InCharacter;
	targIndex = 0;

	for (ATargetPoint* Target : Targets)
	{
		UE_LOG(LogTemp, Display, TEXT("Target: %s"), *Target->GetActorLocation().ToString())
	}
}

void APipeTravel::Travel(float DeltaTime)
{
	//checks
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player actor not found, perhaps it wasn't passed through"));
		return;
	}
	if (Targets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pipe %s requrires at least 1 target"), *GetName());
		return;
	}
	if (targIndex == 0)
	{
		ATargetPoint* TargetPoint = Targets[targIndex];
		FVector TargetLocation = TargetPoint->GetActorLocation();
		Player->SetActorLocation(TargetLocation);
		targIndex++;
		return;
	}

	//stores where the player currently is
	FVector CurrentLocation = Player->GetActorLocation();

	//stores where the player is meant to be going
	ATargetPoint* TargetPoint = Targets[targIndex];
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
		Player->SetActorLocation(TargetLocation);

		//increment target index
		targIndex++;

		//if there are no more targets to travel to, eject
		if (targIndex >= Targets.Num())
		{
			bIsTravelling = false;
			DirectionVector.Normalize();
			Eject(Player, DirectionVector, EjectForce); //blueprint function
			Player = nullptr;
		}
	}
	else
	{
		//set player location to move closer to target
		FVector NewPosition = CurrentLocation + DirectionVector;
		Player->SetActorLocation(NewPosition);
	}
	
}
