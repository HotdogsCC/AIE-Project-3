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

	virtual void DeclarePlayer(AAIEProject3Character* PlayerPointer) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateDeadWidget(AAIEProject3Character* PlayerPointer);

	UFUNCTION(BlueprintCallable)
	void SetPlayerWithSafeGuard(AAIEProject3Character* PlayerPointer);

	//The blueprint of the text asset
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATextRenderActor> SafeGuardTextBlueprint;

	//the instance of the blueprint
	AActor* SafeGuardTextActor;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	uint8 InitialNumOfPlayers = 0;

	void SwapPlayers(uint8 NumOfPlayers);

	AAIEProject3Character* SafeGuardedPlayer = nullptr;

	void ProcessSafeGuard();
	
};
