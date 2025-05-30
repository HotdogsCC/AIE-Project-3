// Fill out your copyright notice in the Description page of Project Settings.


#include "MinigameHotPotato.h"

#include "GeometryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/TextRenderActor.h"
#include "MinigameCharacterBase.h"
#include "MainGameInstance.h"
#include "PipeTravel.h"
#include "Components/AudioComponent.h"

//called on first frame
void AMinigameHotPotato::BeginPlay()
{
	Super::BeginPlay();
	
	EndMode = FEndMode::LASTPLAYER;
	InitTimeLimit = TimeLimit;

	//check there is a valid reference to tagged text
	if(!TaggedTextBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("There was no Tagged Text blueprint attached to the minigame hot potato game mode"));
		return;
	}

	//spawns the tagged text into the world
	TaggedTextActor = GetWorld()->SpawnActor(TaggedTextBlueprint);

	const FVector MiddleOfNowhere(9999, 9999, 9999);
	
	//spawns all of the highlights into the world
	for (int32 i = 0; i < 4; i++)
	{
		AActor* Highlight = GetWorld()->SpawnActor(PlayerHighlightBP);
		Highlight->SetActorLocation(MiddleOfNowhere);
		PlayerHighlighters.Add(Highlight);
	}
	
	AssignTagged();

	//plays the ticking sound
	TickSoundPlayback = UGameplayStatics::CreateSound2D(this, BombTickSound);
	if(TickSoundPlayback)
	{
		TickSoundPlayback->Play();
	}
	
	
}

void AMinigameHotPotato::AssignTagged()
{
	//gets player indexs of players alive	
	TArray<uint8> PlayersIndexesThatAreAlive;
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	for (uint8 i = 0; i < 4; i++)
	{
		if (GameInstance->IsPlayerAlive(i))
		{
			UE_LOG(LogTemp, Display, TEXT("Player %i is alive"), i);
			PlayersIndexesThatAreAlive.Add(i);
		}
	}

	//picks a random index from that array
	const uint8 MaxPlayerIndex = PlayersIndexesThatAreAlive.Num() - 1;
	const uint8 IndexForPlayersAlive = FMath::RandRange(0, MaxPlayerIndex);
	const uint8 PlayerIndex = PlayersIndexesThatAreAlive[IndexForPlayersAlive];


	//Get the player from that index
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, PlayerIndex);

	//check the pointer is valid
	if(!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller in Minigame Hot Potato is null"));
		return;
	}

	//Set the tagged player from the controller
	TaggedPlayer = Cast<AMinigameCharacterBase>(PlayerController->GetPawn());

	//make sure the tagged player is not null
	if(!TaggedPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tagged Player in Minigame Hot Potato is null"));
		return;
	}
	
}

void AMinigameHotPotato::Tick(float DeltaTime)
{
	//check tagged player is real
	if(!TaggedPlayer || !TaggedTextActor)
	{
		return;
	}

	//updates timer (found in parent)
	UpdateTimer(DeltaTime);

	//sets tagged text position
	const FVector PlayerLocation = TaggedPlayer->GetActorLocation();
	TaggedTextActor->SetActorLocation(PlayerLocation);

	if(TimeLimit <= 5)
	{
		if(!bFastSoundHasPlayed)
		{
			bFastSoundHasPlayed = true;
			//plays fast scary sound
			UGameplayStatics::PlaySound2D(this, FastBombTickSound);

			//stops the other ticking sound
			if(TickSoundPlayback)
			{
				TickSoundPlayback->Stop();
			}

			//changes the flashing actor
			TaggedTextActor->Destroy();
			TaggedTextActor = GetWorld()->SpawnActor(TaggedTextBlueprintFast);

			
		}
		//checks if time has run out
		if(TimeLimit <= 0)
		{
			for (int32 i = 0; i < Pipes.Num(); i++) 
			{
				if (Pipes[i])
				{
					Pipes[i]->SetPlayerDead(Cast<AActor>(TaggedPlayer));
				}
			}

			// tell the game mode the tagged player died
			AController* Controller = TaggedPlayer->GetController();
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			DeclareDeadPlayer(PlayerController->GetLocalPlayer()->GetControllerId());

		
		
			//kill the tagged player
			TaggedPlayer->Destroy();
			TaggedPlayer = nullptr;

			//tell the tagged text actor to fuck off
			FVector MiddleOfNowhere(0.0f, 0.0f, -99999.0f);
			TaggedTextActor->SetActorLocation(MiddleOfNowhere);

			//changes the flashing actor
			TaggedTextActor->Destroy();
			TaggedTextActor = GetWorld()->SpawnActor(TaggedTextBlueprint);

			//reset for another round
			TimeLimit = InitTimeLimit;
			AssignTagged();

			//plays the ticking sound
			TickSoundPlayback = UGameplayStatics::CreateSound2D(this, BombTickSound);
			if(TickSoundPlayback)
			{
				bFastSoundHasPlayed = false;
				TickSoundPlayback->Play();
			}
		}
	}
	
	
	
}


//Called when two players collide
void AMinigameHotPotato::PlayerCollision(AMinigameCharacterBase* Character1, AMinigameCharacterBase* Character2)
{
	//check that we have legit pointers
	if (!Character1 || !Character2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Null pointer for characters in MinigameHotPotato PlayerCollision"));
		return;
	}

	//if one of the characters are tagged, swap their tag status
	if (TaggedPlayer == Character1 || TaggedPlayer == Character2)
	{
		TaggedPlayer = (TaggedPlayer == Character1) ? Character2 : Character1;
	}
}

void AMinigameHotPotato::AddPipe(APipeTravel* Pipe) 
{
	Pipes.Add(Pipe);
}

AActor* AMinigameHotPotato::GetPlayerHighlighter(int32 i) const
{
	return PlayerHighlighters[i];
}