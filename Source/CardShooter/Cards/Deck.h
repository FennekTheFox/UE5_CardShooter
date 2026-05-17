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

//Used for decks that are in the players collection -> Card identities come from the player save file
USTRUCT(BlueprintType)
struct FCardDeck
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FString DeckName;

	//Set utility cards that need not be drawn and can be played at any time
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TArray<FCardIdentityHandle> UtilityCards;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	TArray<FCardIdentityHandle> CardHandles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FLinearColor DeckColor;

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
	TArray<FCardIdentity> CardList;

	UFUNCTION(BlueprintPure)
	void GetDeck(FCardDeck& OutDeck);
};