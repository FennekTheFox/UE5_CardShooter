#pragma once

#include "Cards/Deck.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "Cards/CardIdentity.h"
#include "Cards/Augments/CardAugmentShard.h"

#include "CardShooterSaveData.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCollectionCardModified, const FCardIdentityHandle&, Handle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyCardChanged);

USTRUCT(BlueprintType)
struct FCardCollection 
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FCardIdentity> Cards;
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
	int32 LastSelectedDeckIndex = -1;

private:
	UPROPERTY(SaveGame)
	FCardCollection Collection;

	UPROPERTY(SaveGame)
	TMap<FCardAugmentShard, int32> AugmentShards;

	UPROPERTY(SaveGame)
	TArray<FCardDeck> DeckList;

	UPROPERTY(SaveGame)
	TMap<FGameplayTag, int32> Currencies;

public:
	UFUNCTION(BlueprintCallable)
	void ValidateAndFix(FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintPure)
	int32 GetCardCount(const FCardIdentity& CarDIdentity) const;

	UFUNCTION(BlueprintCallable)
	bool GetCardFromCollection(const FCardIdentityHandle& Handle, FCardIdentity& OutCard) const;

	UFUNCTION(BlueprintCallable)
	FCardIdentityHandle GetFirstCardHandle(const FCardIdentity& CardIdentity) const;
	UFUNCTION(BlueprintCallable)
	void GetAllCardHandles(const FCardIdentity& CardIdentity, TArray<FCardIdentityHandle>& CardHandles) const;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void ModifyCardInCollection(UObject* WorldContext, const FCardIdentityHandle& Handle, const FCardIdentity& InCard);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void AddCardToCollection(UObject* WorldContext, const FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	bool RemoveCardFromCollection(UObject* WorldContext, const FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintCallable)
	void GetSortedCardCollectionKeys(TArray<FCardIdentity>& OutCards) const;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	bool AssignCardToDeck(UObject* WorldContext, const FCardIdentityHandle& CardHandle, int32 DeckHandle);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	bool UnassignCardFromDeck(UObject* WorldContext, const FCardIdentityHandle& Handle, int32 DeckIndex);
	UFUNCTION(BlueprintCallable)
	bool UpdateDeckAtIndex(const FCardDeck& UpdatedDeck, int32 DeckHandle);
	UFUNCTION(BlueprintCallable)
	void AddNewDeck(const FCardDeck& NewDeck);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	bool DeleteDeckAtIndex(UObject* WorldContext, int32 DeckHandle);
	UFUNCTION(BlueprintCallable)
	void GetAllDecks(TArray<FCardDeck>& OutDecks) const;
	UFUNCTION(BlueprintCallable)
	bool GetDeckAtIndex(int32 DeckIndex, FCardDeck& OutDeck) const;

	UFUNCTION(BlueprintCallable)
	void AddCurrency(FGameplayTag CurrencyTag, int32 Amount);
	UFUNCTION(BlueprintCallable)
	bool DeductCurrency(FGameplayTag CurrencyTag, int32 Amount);
	UFUNCTION(BlueprintCallable)
	int32 GetCurrencyAmount(FGameplayTag CurrencyTag) const;

	UFUNCTION(BlueprintPure)
	bool HasSufficientCurrency(const FGameplayTag CurrencyTag, int32 RequiredAmount) const;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void AddShard(UObject* WorldContext, const FCardAugmentShard& Shard);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	bool RemoveShard(UObject* WorldContext, const FCardAugmentShard& Shard);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void GetAllShards(const TMap<FCardAugmentShard, int32>& Shards);



	UPROPERTY(BlueprintAssignable)
	FOnAnyCardChanged OnAnyCardChanged;
	FTimerHandle AnyCardChangedTimer;
	void CallAnyCardChangedNextTick(UObject* WorldContext);
	UFUNCTION(BlueprintCallable)
	void BindEventToOnCollectionCardModified(const FCardIdentityHandle& Handle, FOnCollectionCardModified Callback);
	void CallOnCollectionCardModifiedCallback(const FCardIdentityHandle& Handle);
	void CleanUpOnCardModifiedCallbacks(const FCardIdentityHandle& Handle);
	TMap<FCardIdentityHandle, TArray<FOnCollectionCardModified>> OnCardModifiedCallbacks;
};