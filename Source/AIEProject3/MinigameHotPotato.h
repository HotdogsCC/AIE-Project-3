// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinigameGameModeBase.h"
#include "MinigameHotPotato.generated.h"

class ATextRenderActor;
/**
 * 
 */
UCLASS()
class AIEPROJECT3_API AMinigameHotPotato : public AMinigameGameModeBase
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	///Called when two players collide
	virtual void PlayerCollision(AAIEProject3Character* Character1, AAIEProject3Character* Character2) override;

	//The blueprint of the text asset
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATextRenderActor> TaggedTextBlueprint;

	AActor* TaggedTextActor;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//sets a player as tagged
	void AssignTagged();
	
	//Stores a pointer to the player that is currently it
	AAIEProject3Character* TaggedPlayer;
};
