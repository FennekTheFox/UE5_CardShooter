#pragma once

#include "Cards/Deck.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"

#include "CardShooterSaveData.generated.h"


USTRUCT(BlueprintType)
struct FCardCollection 
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<FCardIdentity, int32> Cards;
};

UCLASS(BlueprintType, Blueprintable)
class UCardShooterSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (ExposeOnSpawn))
	FString ProfileName;

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

	//TODO Currencies


	UPROPERTY(SaveGame, BlueprintReadWrite)
	int32 LastSelectedDeckIndex = -1;


public:
	UFUNCTION(BlueprintCallable)
	void ValidateAndFix(FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintCallable)
	void AddCardToCollection(const FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintCallable)
	bool RemoveCardFromCollection(const FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintCallable)
	void GetSortedCardCollectionKeys(TArray<FCardIdentity>& OutCards);
};