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

	//Called by blueprint when a player enters the zone
	UFUNCTION(BlueprintCallable)
	void StartTravelling();
};
