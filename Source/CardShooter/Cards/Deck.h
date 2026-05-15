#pragma once

#include "CardData.h"
#include "Deck.generated.h"

class UCardData;

//Used for verifying deck limits, so we dont have to look up all the data assets all the time
USTRUCT(BlueprintType)
struct FCardDeck_DataOnly
{
	GENERATED_BODY()

	//Set utility cards that need not be drawn and can be played at any time
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TArray<TObjectPtr<UCardData>> UtilityCards;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TArray<TObjectPtr<UCardData>> CardList;

	//If true, the deck does not get checked for card availability in collection
	//TODO
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bDebugDeck = false;
};


USTRUCT(BlueprintType)
struct FCardDeck
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FString DeckName;

	//Set utility cards that need not be drawn and can be played at any time
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TArray<FCardIdentity> UtilityCards;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TArray<FCardIdentity> CardList;

	//If true, the deck does not get checked for card availability in collection
	//TODO
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bDebugDeck = false;
};

UCLASS(BlueprintType)
class UPrebuiltDeck : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag DeckTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UCardData>> CardList;

	UFUNCTION(BlueprintPure)
	void GetDeck(FCardDeck& OutDeck);
};