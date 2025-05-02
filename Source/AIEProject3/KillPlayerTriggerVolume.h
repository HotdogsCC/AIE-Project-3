// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "Engine/TriggerVolume.h"
#include "KillPlayerTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class AIEPROJECT3_API AKillPlayerTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	AKillPlayerTriggerVolume();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
