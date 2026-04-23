#pragma once

#include "Deck.h"
#include "GameFramework/SaveGame.h"

#include "CardShooterSaveData.generated.h"


USTRUCT()
struct FCardCollection 
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FGameplayTagContainer CardContainer;
};

UCLASS(BlueprintType, Blueprintable)
class UCardShooterSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FCardCollection Collection;

	UPROPERTY(SaveGame)
	TArray<FCardDeck> DeckList;
};