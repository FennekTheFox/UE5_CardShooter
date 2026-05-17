#pragma once

#include "GameplayTagContainer.h"
#include "Augments/CardAugmentData.h"

#include "CardIdentity.generated.h"

UENUM()
enum class ECardRarity : uint8
{
	Common = 0,
	Rare = 1,
	UltraRare = 2
};


//The general definition of a card instance. UStruct to make replication easier for now
USTRUCT(BlueprintType)
struct FCardIdentity
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Data")
	FGameplayTag AssetTag;

	//This is supposed to make the access to the card faster. If not found because the asset is moved
	//we need to look through all card data assets to find the right one
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Data")
	FSoftObjectPath CardDataPath;

	//Why? Because adding a card to a deck makes it linked to that deck. editing the card  outside of the
	//deck editor needs to also edit it in said deck
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Data")
	TArray<int32> UsedInDecksWithIndices;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame, Category = "Appearance")
	ECardRarity CardRarity = ECardRarity::Common;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame, Category = "Appearance")
	bool bFullArt = false;

	//States the number of augments that can be socketed in this specific card
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	int32 NumAugmentSockets = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	TArray<FCardAugmentData> Augments;


	bool operator== (const FCardIdentity& Other) const
	{
		return AssetTag == Other.AssetTag
			&& CardRarity == Other.CardRarity
			&& bFullArt == Other.bFullArt
			&& NumAugmentSockets == Other.NumAugmentSockets
			&& Augments == Other.Augments
			&& UsedInDecksWithIndices == Other.UsedInDecksWithIndices;
	}

	bool operator!= (const FCardIdentity& Other) const
	{
		return !(*this == Other);
	}

	bool operator<(const FCardIdentity& Other) const;
};

FORCEINLINE uint32 GetTypeHash(const FCardIdentity& Identity)
{
	uint32 Hash = GetTypeHash(Identity.CardDataPath);
	Hash = HashCombine(Hash, GetTypeHash(Identity.CardRarity));
	Hash = HashCombine(Hash, GetTypeHash(Identity.bFullArt));
	return Hash;
}


UENUM(BlueprintType)
enum class ECardIdentityStorageLocation : uint8
{
	Invalid = 0,
	Collection = 1,
	Cache = 2,
	Prebuilt = 3,
};

USTRUCT(BlueprintType)
struct FCardIdentityHandle
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Index = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	ECardIdentityStorageLocation StorageLocation = ECardIdentityStorageLocation::Invalid;

	UPROPERTY(BlueprintReadWrite)
	FSoftObjectPath PrebuiltPath;

	bool IsValid() const
	{
		return Index != INDEX_NONE;
	}

	bool operator==(const FCardIdentityHandle& Other) const
	{
		return Index == Other.Index &&
			StorageLocation == Other.StorageLocation &&
			PrebuiltPath == Other.PrebuiltPath;
	}
};

FORCEINLINE uint32 GetTypeHash(const FCardIdentityHandle& Handle)
{
	uint32 Hash = GetTypeHash(Handle.Index);
	Hash = HashCombine(Hash, GetTypeHash(static_cast<uint8>(Handle.StorageLocation)));
	Hash = HashCombine(Hash, GetTypeHash(Handle.PrebuiltPath));
	return Hash;
}

//Might seem useless but its used as a map value type in blueprints so do not delete
USTRUCT(BlueprintType)
struct FCardIdentityHandleList
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FCardIdentityHandle> Cards;
};


UCLASS()
class CARDSHOOTER_API UCardIdentityHandleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CardShooter|Cards")
	static FCardIdentityHandle GetOrAddCardIdentityHandle(const FCardIdentity& Identity);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CardShooter|Cards")
	static bool GetCardIdentityFromHandle(const FCardIdentityHandle& Handle, FCardIdentity& OutIdentity, APlayerController* PC = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CardShooter|Cards")
	static bool GetCachedCardIdentityFromIndex(const int32& Index, FCardIdentity& OutIdentity);

	UFUNCTION(BlueprintCallable, Category = "CardShooter|Cards")
	static void ClearCardIdentityCache();

private:
	static TArray<FCardIdentity> CachedIdentities;
};