#pragma once

#include "Deck.generated.h"

class UCardData;

USTRUCT(BlueprintType)
struct FCardDeck
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UCardData>> CardList;
};

UCLASS(BlueprintType)
class UPrebuiltDeck : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCardDeck CardList;
};