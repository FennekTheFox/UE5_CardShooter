#pragma once

#include "Deck.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"

#include "CardShooterSaveData.generated.h"


USTRUCT(BlueprintType)
struct FCardCollection 
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FGameplayTagContainer CardContainer;
};

UCLASS(BlueprintType, Blueprintable)
class UCardShooterSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString SaveGameName;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FDateTime DateTimeLastSaved;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FDateTime DateTimeCreated;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FTimespan PlayDuration;


	UPROPERTY(SaveGame, BlueprintReadWrite)
	FGameplayTagContainer ProgressionTags;


	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FGameplayTag, FTimespan> GauntletCompletionTimes;


	UPROPERTY(SaveGame, BlueprintReadWrite)
	FCardCollection Collection;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FCardDeck> DeckList;
};