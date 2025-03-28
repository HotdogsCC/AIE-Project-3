// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "PipeTravel.generated.h"

/**
 * 
 */
UCLASS()
class AIEPROJECT3_API APipeTravel : public ATriggerSphere
{
	GENERATED_BODY()

public:

	APipeTravel();

	virtual void Tick(float DeltaTime) override;

	//Called by blueprint when a player enters the zone
	UFUNCTION(BlueprintCallable)
	void StartTravelling(AActor* InCharacter);

	//Implemented in blueprints, gives player control again
	UFUNCTION(BlueprintImplementableEvent)
	void Eject(AActor* InCharacter, FVector ForceDirection, float Force);

	//Stores the 'keyframes' the player needs to travel to along the pipe
	UPROPERTY(EditInstanceOnly)
	TArray<class ATargetPoint*> Targets;

	//Units per second the hampter will travel through the pipe
	UPROPERTY(EditInstanceOnly)
	float TravelSpeed = 1000.0f;

	UPROPERTY(EditInstanceOnly)
	float EjectForce = 1.0f;

private:
	//runs when travelling
	void Travel(float DeltaTime);

	//Stores a pointer to the player that we are trying to move
	AActor* Player;

	//When true, the script will move the player through the pipe
	bool bIsTravelling;

	//index to the target the player should be moving to
	int targIndex = 0;
};
