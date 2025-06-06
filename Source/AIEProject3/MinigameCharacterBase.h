// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MinigameCharacterBase.generated.h"

UCLASS()
class AIEPROJECT3_API AMinigameCharacterBase : public ACharacter
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	AMinigameCharacterBase();

	/** Called for dashing input */
	virtual void Dash(const struct FInputActionValue& Value);

protected:
	/** Called for movement input */
	virtual void Move(const struct FInputActionValue& Value);

	/** Called for looking input */ 
	virtual void Look(const struct FInputActionValue& Value); 

	

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BallComponent;

	//used for making sure both players bounce in a collision
	bool bShouldBounce = true;
	FTimerHandle BounceResetTimer;
	void ResetBounce();

	FTimerHandle DashResetTimer;

	int temp = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;

	UFUNCTION()
	void ReturnToSelection(const struct FInputActionValue& Value);

	UFUNCTION()
	void NotifyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//bounce force, set it bp
	UPROPERTY(EditDefaultsOnly, Category = "Bounce")
	float Bounciness = 3.0f;

	//bounce force, set it bp
	UPROPERTY(EditDefaultsOnly, Category = "Bounce")
	float MinimumBounce = 10.0f;

	//bounce force, set it bp
	UPROPERTY(EditDefaultsOnly, Category = "Bounce")
	float MaximumBounce = 100.0f;

	//dash force, set it bp
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashForce = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TArray<UMaterialInstance*> CharacterBallMaterials;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* JumpSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* DashSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* BounceSound;

	//used by other actor to see if it should bounce
	bool GetShouldBounce() const { return bShouldBounce; }

private:
	bool bCanDash = true;
public:
	void ResetCanDash() { bCanDash = true; }



};
