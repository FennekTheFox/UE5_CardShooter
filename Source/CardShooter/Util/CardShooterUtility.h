#pragma once

#include "Cards/CardData.h"
#include "Cards/Deck.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"

#include "CardShooterUtility.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGameplayTagChangedBP, FGameplayTag, Tag, int32, Count);

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

	UFUNCTION(BlueprintPure)
	static bool StringLexComparison(const FString& A, const FString& B);

	UFUNCTION(BlueprintCallable)
	static void BindEventToTagCountChanged(class UAbilitySystemComponent* ASC, FGameplayTag Tag, FOnGameplayTagChangedBP Callback);

	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "Class"))
	static class UCardEffectFragment_Attribute* GetAttributeOfClassFromArray(TArray<UCardEffectFragment_Attribute*> Array, TSubclassOf<UCardEffectFragment_Attribute> Class);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Card Identity)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Card")
	static bool Equal_CardIdentity(const FCardIdentity& A, const FCardIdentity& B);
};