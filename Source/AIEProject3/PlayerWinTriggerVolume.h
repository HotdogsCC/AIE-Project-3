// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "PlayerWinTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class AIEPROJECT3_API APlayerWinTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
public:
	APlayerWinTriggerVolume();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
