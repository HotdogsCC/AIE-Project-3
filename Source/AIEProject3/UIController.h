// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UIController.generated.h"

UCLASS()
class AIEPROJECT3_API AUIController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called upon a Select input being triggered by Enhanced Input System (probably A on a controller)
	void Select();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateLobbyScreenWidget(int32 PlayerNumber);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LobbyWidget;

private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectAction;

	

};
