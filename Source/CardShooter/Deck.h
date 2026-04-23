#pragma once

#include "Deck.generated.h"

USTRUCT(BlueprintType)
struct FCardDeck
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UCardData>> CardList;
};