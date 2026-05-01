#include "CardShooterSaveData.h"

void UCardShooterSaveData::MakeUniqueCollectionIdentity(FCardIdentity& CardIdentity)
{
	while (Collection.Cards.Find(CardIdentity))
	{
		CardIdentity.UniqueID += 1;
	}
}

void UCardShooterSaveData::AddCardToCollection(const FCardIdentity& CardIdentity)
{
	Collection.Cards.Add(CardIdentity);
}

void UCardShooterSaveData::RemoveCardFromCollection(const FCardIdentity& CardIdentity)
{
	Collection.Cards.Remove(CardIdentity);
}

