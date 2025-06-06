// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinigameGameModeBase.h"
#include "MinigameDeathSwap.generated.h"

class ATextRenderActor;

/**
 * 
 */
UCLASS()
class AIEPROJECT3_API AMinigameDeathSwap : public AMinigameGameModeBase
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DeclarePlayer(AMinigameCharacterBase* PlayerPointer) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateDeadWidget(AMinigameCharacterBase* PlayerPointer);

	UFUNCTION(BlueprintCallable)
	void SetPlayerWithSafeGuard(AMinigameCharacterBase* PlayerPointer);

	AMinigameCharacterBase* GetPlayerWithSafeGuard() const { return SafeGuardedPlayer; }

	//The blueprint of the text asset
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATextRenderActor> SafeGuardTextBlueprint;

	//Whether the map swaps or not
	UPROPERTY(EditDefaultsOnly)
	bool SwitchBetweenMaps = false;

	//the instance of the blueprint
	AActor* SafeGuardTextActor;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> MapNames;

	///Called when two players collide
	virtual void PlayerCollision(AMinigameCharacterBase* Character1, AMinigameCharacterBase* Character2) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	uint8 InitialNumOfPlayers = 0;

	void SwapPlayers(uint8 NumOfPlayers);

	AMinigameCharacterBase* SafeGuardedPlayer = nullptr;

	void ProcessSafeGuard();

	//the sound to play when swapping is about to begin
	UPROPERTY(EditDefaultsOnly, Category="Sounds", meta=(AllowPrivateAccess=true))
	USoundBase* SwapSound;

	//stops double plays
	bool bHasSoundPlayed = false;
	
};
