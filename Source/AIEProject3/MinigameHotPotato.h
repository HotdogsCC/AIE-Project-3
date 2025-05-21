// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinigameGameModeBase.h"
#include "MinigameHotPotato.generated.h"

class ATextRenderActor;
class AMinigameCharacterBase;
class APipeTravel;
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
	virtual void PlayerCollision(AMinigameCharacterBase* Character1, AMinigameCharacterBase* Character2) override;

	//The blueprint of the text asset
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATextRenderActor> TaggedTextBlueprint;

	AActor* TaggedTextActor;

	void AddPipe(APipeTravel* Pipe);
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//sets a player as tagged
	void AssignTagged();
	
	//Stores a pointer to the player that is currently it
	AMinigameCharacterBase* TaggedPlayer;

	//stores all of the locations along a pipe
	TArray<APipeTravel*> Pipes;

	//bomb ticking, low stress
	UPROPERTY(EditDefaultsOnly, Category="Sounds", meta=(AllowPrivateAccess=true))
	USoundBase* BombTickSound;

	//bomb ticking, high stress
	UPROPERTY(EditDefaultsOnly, Category="Sounds", meta=(AllowPrivateAccess=true))
	USoundBase* FastBombTickSound;

	//current instance of bomb tick playback
	UAudioComponent* TickSoundPlayback = nullptr;

	//prevents lots of sounds
	bool bFastSoundHasPlayed = false;
};
