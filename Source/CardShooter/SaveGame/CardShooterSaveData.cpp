#include "CardShooterSaveData.h"
#include "Util/CardShooterUtility.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Currency.h"
#include "Kismet/GameplayStatics.h"
#include "CardShooterSaveGameSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Algo/Count.h"
#include "../Inventory/Item.h"

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
	if (!(Handle.StorageLocation == ECardIdentityStorageLocation::Collection))
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

void UCardShooterSaveData::AddItem(UObject* WorldContext, UItem* Item)
{
	if (UItem_Stackable* AsStackable = Cast<UItem_Stackable>(Item))
	{
		TObjectPtr<UItem>* ExistingStack = RuntimeItems.FindByPredicate([AsStackable](UItem* Other)
			{
				if (UItem_Stackable* OtherAsStackable = Cast<UItem_Stackable>(Other))
				{
					return AsStackable->SameItem(OtherAsStackable) && !OtherAsStackable->IsFullStack();
				}
				return false;
			});

		if (ExistingStack)
		{
			if (UItem_Stackable* ExistingStackAStackable = Cast<UItem_Stackable>(*ExistingStack))
			{
				int32 Overflow = ExistingStackAStackable->AddToCount(AsStackable->Count);

				while (Overflow > 0)
				{
					UItem_Stackable* NewStack = DuplicateObject<UItem_Stackable>(ExistingStackAStackable, this);
					Overflow = NewStack->OverrideCount(Overflow);
					RuntimeItems.Add(NewStack);
				}
			}
			else
			{
				ensure(false);
			}
		}
		else
		{
			RuntimeItems.Add(Item);
		}
	}
	else
	{
		RuntimeItems.Add(Item);
	}
}


bool UCardShooterSaveData::RemoveItem(UObject* WorldContext, UItem* Item, int32 Count /*= 1*/)
{
	if (!CamRemoveItems(Item, Count))
	{
		return false;
	}
	else
	{
		if (UItem_Stackable* AsStackable = Cast<UItem_Stackable>(Item))
		{
			TArray<TObjectPtr<UItem>> FoundStacks = RuntimeItems.FilterByPredicate([AsStackable](TObjectPtr<UItem> Other)
				{
					return Other->SameItem(AsStackable);
				});

			int32 Remaining = AsStackable->Count * Count;
			int32 i = 0;
			while (Remaining > 0 && i < FoundStacks.Num())
			{
				if (UItem_Stackable* FoundStackAsStackable = Cast<UItem_Stackable>(FoundStacks[i]))
				{
					Remaining -= FoundStackAsStackable->RemoveFromCount(Remaining);
				}

				i++;
			}

			return Remaining <= 0;
		}
		else
		{
			TArray<TObjectPtr<UItem>> FoundItems = RuntimeItems.FilterByPredicate([Item](TObjectPtr<UItem> OtherItem)
				{
					return Item->SameItem(OtherItem);
				});

			for (int32 i = 0; i < Count; i++)
			{
				TObjectPtr<UItem>* ItemToRemove = RuntimeItems.FindByPredicate([Item](TObjectPtr<UItem> Other) {
					return Item->SameItem(Other);
					});

				if (ItemToRemove)
				{
					RuntimeItems.Remove(*ItemToRemove);
				}
				else
				{
					return ensure(false);
				}
			}

			return true;
		}
	}
}

bool UCardShooterSaveData::CamRemoveItems(UItem* Item, int32 Count)
{
	if (UItem_Stackable* AsStackable = Cast<UItem_Stackable>(Item))
	{
		TArray<TObjectPtr<UItem>> FoundStacks = RuntimeItems.FilterByPredicate([AsStackable](TObjectPtr<UItem> Other)
			{
				return Other->SameItem(AsStackable);
			});

		int32 Remaining = AsStackable->Count * Count;
		int32 i = 0;
		while (Remaining > 0 && i < FoundStacks.Num())
		{
			if (UItem_Stackable* FoundStackAsStackable = Cast<UItem_Stackable>(FoundStacks[i]))
			{
				Remaining -= FoundStackAsStackable->Count;
			}

			i++;
		}

		return Remaining <= 0;
	}
	else
	{
		TArray<TObjectPtr<UItem>> FoundItems = RuntimeItems.FilterByPredicate([Item](TObjectPtr<UItem> OtherItem)
			{
				return Item->SameItem(OtherItem);
			});

		return FoundItems.Num() >= Count;
	}
}

void UCardShooterSaveData::GetAllItems(TArray<UItem*>& OutItems)
{
	OutItems = RuntimeItems;
}

void UCardShooterSaveData::BeforeSave()
{
	Items.Empty();
	Items.Reserve(RuntimeItems.Num());

	for (UItem* Item : RuntimeItems)
	{
		if (!IsValid(Item))
		{
			continue;
		}

		FItemSaveData& SaveData = Items.AddDefaulted_GetRef();

		SaveData.ItemClass = Item->GetClass();

		FMemoryWriter MemoryWriter(SaveData.Data, true);
		FItemSaveArchive Archive(MemoryWriter);

		Item->Serialize(Archive);
	}
}

void UCardShooterSaveData::AfterLoad()
{
	RuntimeItems.Empty();
	RuntimeItems.Reserve(Items.Num());

	for (const FItemSaveData& SaveData : Items)
	{
		if (!SaveData.ItemClass)
		{
			continue;
		}

		UItem* Item = NewObject<UItem>(
			this,
			SaveData.ItemClass
		);

		FMemoryReader MemoryReader(SaveData.Data, true);
		FItemSaveArchive Archive(MemoryReader);

		Item->Serialize(Archive);

		RuntimeItems.Add(Item);
	}
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

