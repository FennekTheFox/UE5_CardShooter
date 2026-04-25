#include "CardShooterUtility.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "../Cards/CardAttributeFragments_DeckLimit.h"

UCardData* UCardShooterUtilityLibrary::GetCardDataFromIdentity(const FCardIdentity& Identity)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData;
	AssetRegistry.TryGetAssetByObjectPath(Identity.CardDataPath, AssetData);
	if (AssetData.IsValid())
	{
		//We found a valid card data
		if (UCardData* LoadedCard = Cast<UCardData>(AssetData.GetAsset()))
		{
			if (LoadedCard->AssetTag == Identity.AssetTag)
			{
				return LoadedCard;
			}
		}
	}

	//We couldnt retrieve a valid card by soft path, so lets check if we can find it at all
	TArray<FAssetData> CardAssets;
	AssetRegistry.GetAssetsByClass(UCardData::StaticClass()->GetClassPathName(), CardAssets, true);
	{
		for (FAssetData& AssetDataIt : CardAssets)
		{
			//We found a valid card data
			if (UCardData* LoadedCard = Cast<UCardData>(AssetDataIt.GetAsset()))
			{
				if (LoadedCard->AssetTag == Identity.AssetTag)
				{
					return LoadedCard;
				}
			}
		}
	}

	return nullptr;
}

bool UCardShooterUtilityLibrary::CanAddCardToDeck(const FCardDeck& Deck, UCardData* CardToAdd, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	FailureReasons.Empty();

	if (!CardToAdd)
	{
		return false;
	}

	TArray<UCardData*> PostChangeDeck;
	PostChangeDeck.Add(CardToAdd);

	for (const FCardIdentity& Identity : Deck.CardList)
	{
		PostChangeDeck.Add(GetCardDataFromIdentity(Identity));
	}

	return VerifyDeck(PostChangeDeck, FailureReasons);
}

bool UCardShooterUtilityLibrary::VerifyDeck(const TArray<UCardData*> Deck, TArray<class UDeckLimitationViolation*>& FailureReasons)
{
	bool bSuccess = true;

	for (UCardData* Card : Deck)
	{
		if (!Card)
		{
			return false;
		}

		for (UCardEffectFragment_Attribute* DeckLimitation : Card->CardAttributes)
		{
			if (UCardEffectFragment_DeckLimitation* AsDeckLimitation = Cast<UCardEffectFragment_DeckLimitation>(DeckLimitation))
			{
				bSuccess &= AsDeckLimitation->Verify(Deck, FailureReasons);
			}
		}
	}

	return bSuccess;
}

