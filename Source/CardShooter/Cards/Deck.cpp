#include "Deck.h"
#include "CardIdentity.h"

void UPrebuiltDeck::GetDeck(FCardDeck& OutDeck)
{
	OutDeck.DeckName = DeckTag.ToString();

	OutDeck.CardHandles.Empty();

	int32 i = 0;
	for (const FCardIdentity& CardData : CardList)
	{
		FCardIdentityHandle Handle;
		Handle.StorageLocation = ECardIdentityStorageLocation::Prebuilt;
		Handle.Index = i;
		Handle.PrebuiltPath = FSoftObjectPath(this);

		OutDeck.CardHandles.Add(Handle);

		i++;
	}
}

