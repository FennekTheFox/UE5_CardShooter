#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "CardData.generated.h"

class UTexture2D;

UENUM()
enum class ECardRarity : uint8 
{
	Common			=0, 
	Rare			=1,
	UltraRare		=2
};

USTRUCT(BlueprintType)
struct FCardIdentity
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FGameplayTag AssetTag;

	//This is supposed to make the access to the card faster. If not found because the asset is moved
	//we need to look through all card data assets to find the right one
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FSoftObjectPath CardDataPath;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	ECardRarity CardRarity = ECardRarity::Common;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	bool bFullArt = false;

	bool operator== (const FCardIdentity& Other) const
	{
		return AssetTag == Other.AssetTag 
			&& CardRarity == Other.CardRarity
			&& bFullArt == Other.bFullArt;
	}

	bool operator!= (const FCardIdentity& Other) const
	{
		return !(*this == Other);
	}
};

FORCEINLINE uint32 GetTypeHash(const FCardIdentity& Identity)
{
	uint32 Hash = GetTypeHash(Identity.CardDataPath);
	Hash = HashCombine(Hash, GetTypeHash(Identity.CardRarity));
	Hash = HashCombine(Hash, GetTypeHash(Identity.bFullArt));
	return Hash;
}

USTRUCT(BlueprintType)
struct FCardIdentityList
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FCardIdentity> Cards;
};

class UCardEffectFragment_Attribute;
class UCardEffectFragment_Execution;

UCLASS(BlueprintType, Blueprintable)
class UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CardName;

	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Gameplay.Cards.IDs"))
	FGameplayTag AssetTag;

	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer CardTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UCardEffectFragment_Attribute>> CardAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UCardEffectFragment_Execution> PrimaryExecution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UCardEffectFragment_Execution> SecondaryExecution;

public:
	UFUNCTION(BlueprintPure)
	void GetIdentity(FCardIdentity& Identity);
	UFUNCTION(BlueprintPure)
	TArray<UCardEffectFragment_Attribute*> GetAllAttributesOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class);
	UFUNCTION(BlueprintPure)
	UCardEffectFragment_Attribute* GetAttributeOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class);

	UFUNCTION(BlueprintPure)
	FText GetDescription();

	UFUNCTION(BlueprintPure)
	FText GetAttributesDescription();

	UFUNCTION(BlueprintPure)
	FText GetExecutionsDescription();
};