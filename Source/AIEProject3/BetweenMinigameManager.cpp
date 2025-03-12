// Fill out your copyright notice in the Description page of Project Settings.


#include "BetweenMinigameManager.h"

// Sets default values
ABetweenMinigameManager::ABetweenMinigameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABetweenMinigameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABetweenMinigameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABetweenMinigameManager::SetSplitscreenStatus(bool bIsOn)
{
	//having 'true' disable splitscreen is silly, so there is a layer of abstraction here
	//passing in true enables splitscreen, false turns it off
	GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(!bIsOn);
}

