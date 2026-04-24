#include "CardShooterUtility.h"
#include "AssetRegistry/AssetRegistryModule.h"

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

