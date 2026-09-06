#pragma once

#include "Cards/CardData.h"
#include "Cards/Deck.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"

#include "CardShooterUtility.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGameplayTagChangedBP, FGameplayTag, Tag, int32, Count);

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(
	bool,
	FCardIdentitySortPredicate,
	const FCardIdentity&, A,
	const FCardIdentity&, B
);

UCLASS()
class UCardShooterUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UCardData* GetCardDataFromIdentity(UPARAM()const FCardIdentity& Identity);

	//UFUNCTION(BlueprintCallable)
	//static bool CanAddCardToDeck(UPARAM()const FCardDeck& Deck, FCardIdentity* CardToAdd, TArray<class UDeckLimitationViolation*>& FailureReasons);

	UFUNCTION(BlueprintCallable)
	static bool VerifyDeck(const FCardDeck& Deck, APlayerController* Player, TArray<class UDeckLimitationViolation*>& FailureReasons);

	UFUNCTION(BlueprintPure)
	static bool StringLexComparison(const FString& A, const FString& B);

	UFUNCTION(BlueprintCallable)
	static void BindEventToTagCountChanged(class UAbilitySystemComponent* ASC, FGameplayTag Tag, FOnGameplayTagChangedBP Callback);

	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "Class"))
	static class UCardEffectFragment_Attribute* GetAttributeOfClassFromArray(TArray<UCardEffectFragment_Attribute*> Array, TSubclassOf<UCardEffectFragment_Attribute> Class);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Card Identity)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Card")
	static bool Equal_CardIdentity(const FCardIdentity& A, const FCardIdentity& B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (Card Identity)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Card")
	static bool Equal_CardIdentityHandle(const FCardIdentityHandle& A, const FCardIdentityHandle& B);

	UFUNCTION(BlueprintPure)
	static AActor* GetInstigatorFromEffectHandle(const FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable)
	static void ResetDurationOnEffect(const FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable)
	static void AddSourceToContext(FGameplayEffectContextHandle Handle, UObject* Source);

	UFUNCTION(BlueprintPure)
	static class UCurrencyDef* FindCurrencyDef(FGameplayTag CurrencyTag);

	UFUNCTION(BlueprintCallable, Category = "CardShooter|Sorting")
	static void SortCardIdentityArray(UPARAM(ref) TArray<FCardIdentity>& Array, FCardIdentitySortPredicate Predicate);

	UFUNCTION(BlueprintCallable, Category = "CardShooter|Augments")
	static UCardAugment* GetCardAugmentCDO(const FCardAugmentData& AugmentData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay Tags")
	static void GetChildGameplayTags(FGameplayTag ParentTag, TArray<FGameplayTag>& ChildTags, bool bIncludeParent = false);

	UFUNCTION(BlueprintCallable, Category = "CardShooter|Cards")
	static TArray<FGameplayTag> GetCardSetByCallerTags(const FCardIdentity& Identity);

	UFUNCTION(BlueprintCallable, Category = "CardShooter|Cards")
	static TArray<FGameplayTag> GetCardScalingGroups(const FCardIdentity& Identity);

	UFUNCTION(BlueprintCallable)
	static FText FormatTextForDisplay(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "Utilities|Object", meta = (DeterminesOutputType = "Class", DynamicOutputParam = "OutObjects"))
	static void FilterObjectsByClass(const TArray<UObject*>& Objects, TSubclassOf<UObject> Class, TArray<UObject*>& OutObjects);

	UFUNCTION(BlueprintPure)
	static bool GetSpecFromActiveEffect(const FActiveGameplayEffectHandle ActiveEffect, class UAbilitySystemComponent* OwningASC, FGameplayEffectSpec& OutSpec);

	UFUNCTION(BlueprintPure)
	static void DuplicateSpec(class UAbilitySystemComponent* ASC, const FGameplayEffectSpec& InSpec, FGameplayEffectSpecHandle& OutSpec);

	UFUNCTION(BlueprintCallable, Category = "Utilities", meta = (DeterminesOutputType = "SourceObject", DynamicOutputParam = "ReturnValue"))
	static UObject* DuplicateObjectTyped(UObject* SourceObject, UObject* NewOuter);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Random", meta = (MapParam = "Weights"))
	static int32 WeightedRandomMapIndex(const TMap<int32, float>& Weights);
	DECLARE_FUNCTION(execWeightedRandomMapIndex);

	UFUNCTION(BlueprintPure)
	static bool GetActiveEffectThatGrantedAbility(UGameplayAbility* AbilityInstance, FActiveGameplayEffectHandle& OutActiveEffectHandle);


	UFUNCTION(BlueprintPure)
	static bool GetSetByCallerValueFromSpec(const FGameplayEffectSpec& Spec, FGameplayTag Tag, float& Value);
};