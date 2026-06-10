#include "Deck.h"
#include "CardIdentity.h"

void UPrebuiltDeck::GetDeck(FCardDeck& OutDeck)
{
	OutDeck.DeckName = DeckTag.ToString();

	OutDeck.UtilityCards.Empty();
	OutDeck.CardHandles.Empty();

	for (int32 i = 0; i < UtilityCards.Num(); i++)
	{
		const FCardIdentity& CardData = CardList[i];

		FCardIdentityHandle Handle;
		Handle.StorageLocation = ECardIdentityStorageLocation::PrebuiltUtilityList;
		Handle.Index = i;
		Handle.StoragePath = FSoftObjectPath(this);

		OutDeck.UtilityCards.Add(Handle);
	}

	for (int32 i = 0; i < CardList.Num(); i++)
	{
		const FCardIdentity& CardData = CardList[i];

		FCardIdentityHandle Handle;
		Handle.StorageLocation = ECardIdentityStorageLocation::PrebuiltCardList;
		Handle.Index = i;
		Handle.StoragePath = FSoftObjectPath(this);

		OutDeck.CardHandles.Add(Handle);
	}
}

