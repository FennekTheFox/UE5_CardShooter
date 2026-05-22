#include "CardShooterSaveData.h"
#include "Util/CardShooterUtility.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Currency.h"
#include "Kismet/GameplayStatics.h"
#include "CardShooterSaveGameSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Algo/Count.h"

static FAutoConsoleCommandWithWorldAndArgs GiveCurrencyCmd(
	TEXT("GiveCurrency"),
	TEXT("Give currency to the player: GiveCurrency <Currency> <Amount> <Player Index>"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda(
		[](const TArray<FString>& Args, UWorld* World)
		{
			if (Args.Num() < 3) return;

			int32 Amount = FCString::Atoi(*Args[1]);
			int32 PlayerIndex = FCString::Atoi(*Args[2]);
			FGameplayTag CurrencyTag;

			FAssetRegistryModule& AssetRegistryModule =
				FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

			FARFilter Filter;
			Filter.ClassPaths.Add(UCurrencyDef::StaticClass()->GetClassPathName());
			Filter.bRecursiveClasses = true;

			TArray<FAssetData> Assets;
			AssetRegistryModule.Get().GetAssets(Filter, Assets);

			for (const FAssetData& Asset : Assets)
			{
				FString Value;
				if (Asset.GetTagValue(TEXT("CommandString"), Value))
				{
					if (Value == Args[0])
					{
						UCurrencyDef* CurrencyDef = Cast<UCurrencyDef>(Asset.GetAsset());
						CurrencyTag = CurrencyDef->CurrencyTag;
					}
				}
			}

			if (!CurrencyTag.IsValid()) return;

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, PlayerIndex))
			{
				if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
				{
					UCardShooterSaveGameSubsystem* SaveGameSubsystem = LocalPlayer->GetSubsystem<UCardShooterSaveGameSubsystem>();

					if (UCardShooterSaveData* SaveData = SaveGameSubsystem->GetLocalPlayerSaveData())
					{
						SaveData->AddCurrency(CurrencyTag, Amount);
					}
				}
			}
		}
	)
);


void UCardShooterSaveData::ValidateAndFix(FCardIdentity& CardIdentity)
{
	//Also correct the asset tag in the card identity just to be sure

	UCardData* Data = UCardShooterUtilityLibrary::GetCardDataFromIdentity(CardIdentity);
	if (ensure(Data))
	{
		CardIdentity.AssetTag = Data->AssetTag;
	}
}

int32 UCardShooterSaveData::GetCardCount(const FCardIdentity& CardIdentity) const
{
	return Algo::CountIf(Collection.Cards, [&CardIdentity](const FCardIdentity& ItemCardIdentity)
		{
			return CardIdentity == ItemCardIdentity;
		});
}

bool UCardShooterSaveData::GetCardFromCollection(const FCardIdentityHandle& Handle, FCardIdentity& OutCard) const
{
	if (!ensure(Handle.StorageLocation == ECardIdentityStorageLocation::Collection))
	{
		return false;
	}

	if (ensure(Collection.Cards.IsValidIndex(Handle.Index)))
	{
		OutCard = Collection.Cards[Handle.Index];
		return true;
	}

	return false;
}

FCardIdentityHandle UCardShooterSaveData::GetFirstCardHandle(const FCardIdentity& CardIdentity) const
{
	int32 Index = INDEX_NONE;
	Collection.Cards.Find(CardIdentity, Index);

	ensure(Index != INDEX_NONE);

	FCardIdentityHandle NewHandle;
	NewHandle.StorageLocation = ECardIdentityStorageLocation::Collection;
	NewHandle.Index = Index;

	return NewHandle;
}

void UCardShooterSaveData::GetAllCardHandles(const FCardIdentity& CardIdentity, TArray<FCardIdentityHandle>& CardHandles) const
{
	for (int32 i = 0; i < Collection.Cards.Num(); i++)
	{
		const FCardIdentity& CardIdentityIt = Collection.Cards[i];

		if (CardIdentity == CardIdentityIt)
		{
			FCardIdentityHandle NewHandle;
			NewHandle.StorageLocation = ECardIdentityStorageLocation::Collection;
			NewHandle.Index = i;

			CardHandles.Add(NewHandle);
		}
	}
}

void UCardShooterSaveData::ModifyCardInCollection(UObject* WorldContext, const FCardIdentityHandle& Handle, const FCardIdentity& InCard)
{
	if (!ensure(Handle.StorageLocation == ECardIdentityStorageLocation::Collection))
	{
		return;
	}

	FCardIdentity Copy = InCard;
	FCardIdentity FromCollection;

	if (!ensure(GetCardFromCollection(Handle, FromCollection)))
	{
		return;
	}

	Copy.UsedInDecksWithIndices = FromCollection.UsedInDecksWithIndices;

	Collection.Cards[Handle.Index] = Copy;

	CallOnCollectionCardModifiedCallback(Handle);
}

void UCardShooterSaveData::AddCardToCollection(UObject* WorldContext, const FCardIdentity& CardIdentity)
{
	FCardIdentity FixedIdentity = CardIdentity;
	ValidateAndFix(FixedIdentity);
	CallAnyCardChangedNextTick(WorldContext);

	Collection.Cards.Add(FixedIdentity);
}

bool UCardShooterSaveData::RemoveCardFromCollection(UObject* WorldContext, const FCardIdentity& CardIdentity)
{
	int32 Index = INDEX_NONE;
	Collection.Cards.Find(CardIdentity, Index);

	if (Index == INDEX_NONE)
	{
		return false;
	}

	for (FCardDeck& Deck : DeckList)
	{
		FCardIdentityHandle HandleToRemove;
		HandleToRemove.StorageLocation = ECardIdentityStorageLocation::Collection;
		HandleToRemove.Index = Index;
		Deck.CardHandles.Remove(HandleToRemove);
	}

	FCardIdentity FixedIdentity = CardIdentity;
	ValidateAndFix(FixedIdentity);
	CallAnyCardChangedNextTick(WorldContext);

	return Collection.Cards.RemoveSingle(CardIdentity) == 1;
}

void UCardShooterSaveData::GetSortedCardCollectionKeys(TArray<FCardIdentity>& OutCards) const
{
	TSet<FCardIdentity> CardIDSet(Collection.Cards);
	OutCards = CardIDSet.Array();

	OutCards.Sort([](const FCardIdentity& A, const FCardIdentity& B) {
		return A < B;
		});
}

bool UCardShooterSaveData::AssignCardToDeck(UObject* WorldContext, const FCardIdentityHandle& CardHandle, int32 DeckHandle)
{
	if (!ensure(CardHandle.StorageLocation == ECardIdentityStorageLocation::Collection))
	{
		return false;
	}

	if (!ensure(Collection.Cards.IsValidIndex(CardHandle.Index)))
	{
		return false;
	}

	Collection.Cards[CardHandle.Index].UsedInDecksWithIndices.AddUnique(DeckHandle);
	CallOnCollectionCardModifiedCallback(CardHandle);
	CallAnyCardChangedNextTick(WorldContext);

	return true;
}

bool UCardShooterSaveData::UnassignCardFromDeck(UObject* WorldContext, const FCardIdentityHandle& Handle, int32 DeckIndex)
{
	if (!ensure(Handle.StorageLocation == ECardIdentityStorageLocation::Collection))
	{
		return false;
	}

	if (!ensure(Collection.Cards.IsValidIndex(Handle.Index)))
	{
		return false;
	}

	Collection.Cards[Handle.Index].UsedInDecksWithIndices.Remove(DeckIndex);
	CallOnCollectionCardModifiedCallback(Handle);
	CallAnyCardChangedNextTick(WorldContext);

	return true;
}

bool UCardShooterSaveData::UpdateDeckAtIndex(const FCardDeck& UpdatedDeck, int32 DeckHandle)
{
	if (!ensure(DeckList.IsValidIndex(DeckHandle)))
	{
		return false;
	}

	DeckList[DeckHandle] = UpdatedDeck;

	return true;
}


void UCardShooterSaveData::AddNewDeck(const FCardDeck& NewDeck)
{
	DeckList.Add(NewDeck);
}

bool UCardShooterSaveData::DeleteDeckAtIndex(UObject* WorldContext, int32 DeckHandle)
{
	if (!ensure(DeckList.IsValidIndex(DeckHandle)))
	{
		return false;
	}

	DeckList.RemoveAt(DeckHandle);

	//Sanitize all cards

	for (int CardIndex = 0; CardIndex < Collection.Cards.Num(); CardIndex++)
	{
		const FCardIdentity& Card = Collection.Cards[CardIndex];
		for (int i = 0; i < Card.UsedInDecksWithIndices.Num(); i++)
		{
			const int32 AssignedDeck = Card.UsedInDecksWithIndices[i];

			if (AssignedDeck >= DeckHandle)
			{
				FCardIdentityHandle OutHandle;
				OutHandle.StorageLocation = ECardIdentityStorageLocation::Collection;
				OutHandle.Index = CardIndex;

				UnassignCardFromDeck(WorldContext, OutHandle, AssignedDeck);
				i--;
			}

			if (AssignedDeck > DeckHandle)
			{
				FCardIdentityHandle OutHandle;
				OutHandle.StorageLocation = ECardIdentityStorageLocation::Collection;
				OutHandle.Index = CardIndex;

				AssignCardToDeck(WorldContext, OutHandle, AssignedDeck - 1);
				i++;
			}
		}
	}

	return true;
}

void UCardShooterSaveData::GetAllDecks(TArray<FCardDeck>& OutDecks) const
{
	OutDecks = DeckList;
}

bool UCardShooterSaveData::GetDeckAtIndex(int32 DeckIndex, FCardDeck& OutDeck) const
{
	if (!(DeckList.IsValidIndex(DeckIndex)))
	{
		return false;
	}

	OutDeck = DeckList[DeckIndex];
	return true;
}

void UCardShooterSaveData::AddCurrency(FGameplayTag CurrencyTag, int32 Amount)
{
	int32& HaveAmount = Currencies.FindOrAdd(CurrencyTag, 0);
	HaveAmount += Amount;
}

bool UCardShooterSaveData::DeductCurrency(FGameplayTag CurrencyTag, int32 Amount)
{
	int32& HaveAmount = Currencies.FindOrAdd(CurrencyTag, 0);

	if (HaveAmount >= Amount)
	{
		HaveAmount -= Amount;
		return true;
	}

	return false;
}

int32 UCardShooterSaveData::GetCurrencyAmount(FGameplayTag CurrencyTag) const
{
	const int32* HaveAmount = Currencies.Find(CurrencyTag);

	if (HaveAmount)
	{
		return *HaveAmount;
	}

	return 0.f;
}

bool UCardShooterSaveData::HasSufficientCurrency(const FGameplayTag CurrencyTag, int32 RequiredAmount) const
{
	const int32* HaveAmout = Currencies.Find(CurrencyTag);

	if (HaveAmout)
	{
		return *HaveAmout >= RequiredAmount;
	}

	return false;
}

void UCardShooterSaveData::AddShard(UObject* WorldContext, const FCardAugmentShard& Shard)
{
	int32& Count = AugmentShards.FindOrAdd(Shard, 0);
	Count++;
}

bool UCardShooterSaveData::RemoveShard(UObject* WorldContext, const FCardAugmentShard& Shard)
{
	if (int32* Count = AugmentShards.Find(Shard))
	{
		if (*Count <= 1)
		{
			AugmentShards.Remove(Shard);
			return true;
		}
		else
		{
			*Count--;
			return true;
		}
	}

	return false;
}

void UCardShooterSaveData::GetAllShards(const TMap<FCardAugmentShard, int32>& OutShards)
{
	OutShards;
}

void UCardShooterSaveData::CallAnyCardChangedNextTick(UObject* WorldContext)
{
	if (!AnyCardChangedTimer.IsValid())
	{
		AnyCardChangedTimer = WorldContext->GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
			OnAnyCardChanged.Broadcast();
			AnyCardChangedTimer.Invalidate();
			});
	}
}

void UCardShooterSaveData::BindEventToOnCollectionCardModified(const FCardIdentityHandle& Handle, FOnCollectionCardModified Callback)
{
	if (!ensure(Handle.StorageLocation == ECardIdentityStorageLocation::Collection))
	{
		return;
	}

	if (!ensure(Collection.Cards.IsValidIndex(Handle.Index)))
	{
		ensure(false);
		return;
	}

	CleanUpOnCardModifiedCallbacks(Handle);

	auto& Callbacks = OnCardModifiedCallbacks.FindOrAdd(Handle);
	Callbacks.Add(Callback);
}

void UCardShooterSaveData::CallOnCollectionCardModifiedCallback(const FCardIdentityHandle& Handle)
{
	if (!ensure(Handle.StorageLocation == ECardIdentityStorageLocation::Collection))
	{
		return;
	}

	if (TArray<FOnCollectionCardModified>* Callbacks =
		OnCardModifiedCallbacks.Find(Handle))
	{
		for (FOnCollectionCardModified& Callback : *Callbacks)
		{
			Callback.ExecuteIfBound(Handle);
		}
	}
}

void UCardShooterSaveData::CleanUpOnCardModifiedCallbacks(const FCardIdentityHandle& Handle)
{
	if (TArray<FOnCollectionCardModified>* Callbacks =
		OnCardModifiedCallbacks.Find(Handle))
	{
		Callbacks->RemoveAll(
			[](const FOnCollectionCardModified& Callback)
			{
				return !Callback.IsBound();
			});
	}
}

