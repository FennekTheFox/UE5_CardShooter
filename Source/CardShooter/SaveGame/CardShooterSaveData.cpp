#include "CardShooterSaveData.h"
#include "Util/CardShooterUtility.h"

void UCardShooterSaveData::ValidateAndFix(FCardIdentity& CardIdentity)
{
	//Also correct the asset tag in the card identity just to be sure

	UCardData* Data = UCardShooterUtilityLibrary::GetCardDataFromIdentity(CardIdentity);
	if (ensure(Data))
	{
		CardIdentity.AssetTag = Data->AssetTag;
	}
}

void UCardShooterSaveData::AddCardToCollection(const FCardIdentity& CardIdentity)
{
	FCardIdentity FixedIdentity = CardIdentity;
	ValidateAndFix(FixedIdentity);

	int32& Count = Collection.Cards.FindOrAdd(FixedIdentity, 0);
	Count += 1;
}

bool UCardShooterSaveData::RemoveCardFromCollection(const FCardIdentity& CardIdentity)
{
	FCardIdentity FixedIdentity = CardIdentity;
	ValidateAndFix(FixedIdentity);

	if (int32* Count = Collection.Cards.Find(FixedIdentity))
	{
		if (*Count == 1)
		{
			Collection.Cards.Remove(FixedIdentity);
		}
		else
		{
			Collection.Cards.Add(FixedIdentity, *Count - 1);
		}

		return true;
	}
	else
	{
		//Card not contained
		return false;
	}
}

void UCardShooterSaveData::GetSortedCardCollectionKeys(TArray<FCardIdentity>& OutCards)
{
	Collection.Cards.GetKeys(OutCards);

	OutCards.Sort([](const FCardIdentity& A, const FCardIdentity& B) {
		if (A.AssetTag != B.AssetTag)
		{
			return A.AssetTag.ToString() < B.AssetTag.ToString();
		}
		if (A.CardRarity != B.CardRarity)
		{
			return A.CardRarity < B.CardRarity;
		}
		if (A.bFullArt != B.bFullArt)
		{
			return B.bFullArt;
		}

		ensure(false);
		return false;
		});
}

