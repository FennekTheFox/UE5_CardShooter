#include "Deck.h"

void UPrebuiltDeck::GetDeck(FCardDeck& OutDeck)
{
	OutDeck.DeckName = DeckTag.ToString();

	OutDeck.CardList.Empty();

	for(UCardData* CardData : CardList)
	{
		if (CardData)
		{
			FCardIdentity Identity;
			CardData->GetIdentity(Identity);
			OutDeck.CardList.Add(Identity);
		}
	}
}

