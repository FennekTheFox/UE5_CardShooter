#include "CardIdentity.h"
#include "Deck.h"
#include "SaveGame/CardShooterSaveGameSubsystem.h"
#include "SaveGame/CardShooterSaveData.h"

FCardIdentityHandle FCardIdentityHandle::Invalid = FCardIdentityHandle();

bool FCardIdentity::operator<(const FCardIdentity& Other) const

{
	if (AssetTag != Other.AssetTag)
	{
		return AssetTag.ToString() < Other.AssetTag.ToString();
	}

	if (CardDataPath != Other.CardDataPath)
	{
		return CardDataPath.ToString() < Other.CardDataPath.ToString();
	}

	//full art status superceeds card rarity
	if (bFullArt != Other.bFullArt)
	{
		return bFullArt < Other.bFullArt;
	}

	//visual rarity superceeds augments
	if (CardRarity != Other.CardRarity)
	{
		return static_cast<uint8>(CardRarity) < static_cast<uint8>(Other.CardRarity);
	}

	//Deck assignment superceeds visual rarity
	if (UsedInDecksWithIndices.Num() != Other.UsedInDecksWithIndices.Num())
	{
		return UsedInDecksWithIndices.Num() < Other.UsedInDecksWithIndices.Num();
	}

	for (int32 i = 0; i < UsedInDecksWithIndices.Num(); ++i)
	{
		if (!(UsedInDecksWithIndices[i] == Other.UsedInDecksWithIndices[i]))
		{
			return UsedInDecksWithIndices[i] < Other.UsedInDecksWithIndices[i];
		}
	}

	if (NumAugmentSockets != Other.NumAugmentSockets)
	{
		return NumAugmentSockets < Other.NumAugmentSockets;
	}

	if (Augments.Num() != Other.Augments.Num())
	{
		return Augments.Num() < Other.Augments.Num();
	}

	for (int32 i = 0; i < Augments.Num(); ++i)
	{
		if (!(Augments[i] == Other.Augments[i]))
		{
			return Augments[i] < Other.Augments[i];
		}
	}

	ensure(false);
	return false;
}

TArray<FCardIdentity> UCardIdentityHandleLibrary::CachedIdentities;

FCardIdentityHandle UCardIdentityHandleLibrary::GetOrAddCardIdentityHandle(
	const FCardIdentity& Identity)
{
	for (int32 i = 0; i < CachedIdentities.Num(); ++i)
	{
		if (CachedIdentities[i] == Identity)
		{
			FCardIdentityHandle OutHandle;
			OutHandle.StorageLocation = ECardIdentityStorageLocation::Cache;
			OutHandle.Index = i;
			return OutHandle;
		}
	}

	const int32 NewIndex = CachedIdentities.Add(Identity);
	FCardIdentityHandle OutHandle;
	OutHandle.StorageLocation = ECardIdentityStorageLocation::Cache;
	OutHandle.Index = NewIndex;
	return OutHandle;
}

bool UCardIdentityHandleLibrary::GetCardIdentityFromHandle(
	const FCardIdentityHandle& Handle,
	FCardIdentity& OutIdentity,
	APlayerController* PC /*= nullptr*/)
{
	switch (Handle.StorageLocation)
	{
	case ECardIdentityStorageLocation::Invalid: ensure(false); return false;
	case ECardIdentityStorageLocation::Cache: return GetCachedCardIdentityFromIndex(Handle.Index, OutIdentity);
	case ECardIdentityStorageLocation::Collection:
	{
		UCardShooterSaveData* SaveData = Cast<UCardShooterSaveData>(Handle.StoragePath.TryLoad());
		if (SaveData)
		{
			SaveData->GetCardFromCollection(Handle, OutIdentity);
			return true;
		}

		//if the StoragePath cannot be loaded, it is assumed the data was the persistent save data, not transient game mode data

		if (ensure(PC))
		{
			ULocalPlayer* LP = PC->GetLocalPlayer();
			if (ensure(LP))
			{
				return LP->GetSubsystem<UCardShooterSaveGameSubsystem>()->GetLocalPlayerSaveData()->GetCardFromCollection(Handle, OutIdentity);
			}
			return SaveData->GetCardFromCollection(Handle, OutIdentity);
		}


		return false;
	}
	}

	UPrebuiltDeck* PrebuiltDeck = Cast<UPrebuiltDeck>(Handle.StoragePath.TryLoad());

	switch (Handle.StorageLocation)
	{
	case ECardIdentityStorageLocation::PrebuiltCardList:
		if (!PrebuiltDeck) { return false; }
		if (!PrebuiltDeck->CardList.IsValidIndex(Handle.Index))	{return false;}
		OutIdentity = PrebuiltDeck->CardList[Handle.Index];
		return true;
	case ECardIdentityStorageLocation::PrebuiltUtilityList:
		if (!PrebuiltDeck){	return false;}
		if (!PrebuiltDeck->CardList.IsValidIndex(Handle.Index))	{return false;}
		OutIdentity = PrebuiltDeck->UtilityCards[Handle.Index];
		return true;
	}
	return false;
}

bool UCardIdentityHandleLibrary::GetCachedCardIdentityFromIndex(const int32& Index, FCardIdentity& OutIdentity)
{
	if (!CachedIdentities.IsValidIndex(Index))
	{
		return false;
	}

	OutIdentity = CachedIdentities[Index];
	return true;
}

void UCardIdentityHandleLibrary::ClearCardIdentityCache()
{
	CachedIdentities.Reset();
}