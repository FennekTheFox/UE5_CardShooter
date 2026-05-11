#pragma once

#include "CardData.h"
#include "Deck.generated.h"

class UCardData;

USTRUCT(BlueprintType)
struct FCardDeck
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FString DeckName;

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