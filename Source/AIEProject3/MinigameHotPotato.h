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
	TSubclassOf<AActor> TaggedTextBlueprint;
	//The blueprint of the text asset
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> TaggedTextBlueprintFast;

	//the multiplier of the speed
	UPROPERTY(EditDefaultsOnly)
	float SpeedMultiplier = 1.5f;

	AActor* TaggedTextActor;

	void AddPipe(APipeTravel* Pipe);

	//returns the player highlighter object bound to a player
	AActor* GetPlayerHighlighter(int32 i) const;
	

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

	//the outline object that needs to move with the player to highlight 
	UPROPERTY(EditDefaultsOnly, Category = "Highlights", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> PlayerHighlightBP0;
	UPROPERTY(EditDefaultsOnly, Category = "Highlights", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> PlayerHighlightBP1;
	UPROPERTY(EditDefaultsOnly, Category = "Highlights", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> PlayerHighlightBP2;
	UPROPERTY(EditDefaultsOnly, Category = "Highlights", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> PlayerHighlightBP3;

	//a list for all of the highlights
	TArray<AActor*> PlayerHighlighters;

	//current instance of bomb tick playback
	UAudioComponent* TickSoundPlayback = nullptr;

	//prevents lots of sounds
	bool bFastSoundHasPlayed = false;

	//stores what the regular walk should be
	float RegularWalkSpeed = -400.0f;

	//stores what the boosted walk spped should be
	float TaggedWalkSpeed = -200.0f;
};
