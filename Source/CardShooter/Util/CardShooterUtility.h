#pragma once

#include "Cards/CardData.h"
#include "Cards/Deck.h"

#include "CardShooterUtility.generated.h"

UCLASS()
class UCardShooterUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UCardData* GetCardDataFromIdentity(UPARAM()const FCardIdentity& Identity);

	UFUNCTION(BlueprintCallable)
	static bool CanAddCardToDeck(UPARAM()const FCardDeck& Deck, UCardData* CardToAdd, TArray<class UDeckLimitationViolation*>& FailureReasons);

	UFUNCTION(BlueprintCallable)
	static bool VerifyDeck(const TArray<UCardData*> Deck, TArray<class UDeckLimitationViolation*>& FailureReasons);
};