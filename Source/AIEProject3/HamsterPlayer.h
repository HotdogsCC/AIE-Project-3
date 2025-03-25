// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HamsterPlayer.generated.h"

UCLASS()
class AIEPROJECT3_API AHamsterPlayer : public APawn
{
	GENERATED_BODY()

	//DO NOT USE THIS CLASS
	//DO NOT USE THIS CLASS
	//DO NOT USE THIS CLASS
	//DO NOT USE THIS CLASS
	//DO NOT USE THIS CLASS
	
public:
	// Sets default values for this pawn's properties
	AHamsterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
