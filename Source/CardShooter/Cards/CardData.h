#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Augments/CardAugment.h"
#include "CardIdentity.h"

#include "CardData.generated.h"

class UTexture2D;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Gameplay.Cards.IDs"))
	FGameplayTag AssetTag;

	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer CardTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FText> Dedications;

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
	FText GetDescription(const FCardIdentity& CardIdentity);
	//Helper function mostly to separate out the parameter magnitudes for data validation
	FText GetRawDescription(const FCardIdentity& CardIdentity);
	UFUNCTION(BlueprintPure)
	void GetDedications(FText& OutText) const;

	UFUNCTION(BlueprintPure)
	FText GetAttributesDescription(const FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintPure)
	FText GetExecutionsDescription(const FCardIdentity& CardIdentity);

	UFUNCTION(BlueprintCallable)
	void GetAllUsedParameters(TArray<FGameplayTag>& OutParameters) const;

	UFUNCTION(BlueprintCallable)
	void GetAllUsedScalingGroups(TArray<FGameplayTag>& OutParameters) const;


	UFUNCTION(BlueprintPure)
	static void GetAugments(const FCardIdentity& CardIdentity, TArray<UItem_CardAugment*>& OutAugments);
	UFUNCTION(BlueprintPure)
	static UItem_CardAugment* GetAugmentFromGuid(FGuid Guid);

	UFUNCTION(BlueprintCallable)
	static void SetAugmentGUIDAtIndex(const FCardIdentityHandle& CardHandle, APlayerController* PC, UItem_CardAugment* CardAugment, int32 Index);
};