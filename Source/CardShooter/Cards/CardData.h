#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Augments/CardAugment.h"
#include "CardIdentity.h"
#include "UI/RichTextHoverDecorator.h"

#include "CardData.generated.h"

class UTexture2D;

class UCardEffectFragment_Attribute;
class UCardEffectFragment_Execution;
class UCardParameter;

UCLASS()
class UDedicationData : public UPrimaryDataAsset,
	public IHoverableContext
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText DedicatedTo = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly)
	FText DedicationTitle = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, meta = (MultiLine = "true"))
	FText DedicationText = FText::GetEmpty();

	//BEGIN IHoverableContext
	void GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	void GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	//END IHoverableContext
};

UCLASS(BlueprintType, Blueprintable)
class CARDSHOOTER_API UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CardName;

	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Gameplay.Cards.IDs"))
	FGameplayTag AssetTag;

	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FGameplayTagContainer CardTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<UTexture2D> CardTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TArray<UDedicationData*> Dedications;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Functionality")
	TArray<TObjectPtr<UCardEffectFragment_Attribute>> CardAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Functionality")
	TObjectPtr<UCardEffectFragment_Execution> PrimaryExecution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Functionality")
	TObjectPtr<UCardEffectFragment_Execution> SecondaryExecution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	bool bCheckActivationConditions = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Functionality")
	bool bGloballyEnabled = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "HueSorting")
	float DominantHue = -1.f;

public:
	UFUNCTION(BlueprintPure)
	void GetIdentity(FCardIdentity& Identity);

	UFUNCTION(BlueprintPure)
	void GetCardAttributes(const FCardIdentity& Identity, TArray<UCardEffectFragment_Attribute*>& OutAttributes) const;

	UFUNCTION(BlueprintPure)
	TArray<UCardEffectFragment_Attribute*> GetAllAttributesOfClass(const FCardIdentity& Identity, TSubclassOf<UCardEffectFragment_Attribute> Class);

	UFUNCTION(BlueprintPure)
	UCardEffectFragment_Attribute* GetAttributeOfClass(FCardIdentity& Identity, TSubclassOf<UCardEffectFragment_Attribute> Class);

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
	void GetAllUsedParameterTags(TArray<FGameplayTag>& OutParameters) const;

	UFUNCTION(BlueprintCallable)
	void GetAllUsedParameters(TArray<UCardParameter*>& OutParameters) const;

	UFUNCTION(BlueprintCallable)
	void GetAllUsedScalingGroups(TArray<FGameplayTag>& OutParameters) const;

	void GetParametersByPredicate(TArray<class UCardParameter*>& OutParameters, TFunctionRef<bool(const UCardParameter*)> Predicate) const;

	UFUNCTION(BlueprintPure)
	static void GetAugments(const FCardIdentity& CardIdentity, TArray<UItem_CardAugment*>& OutAugments);
	UFUNCTION(BlueprintPure)
	static UItem_CardAugment* GetAugmentFromGuid(FGuid Guid);

	UFUNCTION(BlueprintCallable)
	static void SetAugmentGUIDAtIndex(const FCardIdentityHandle& CardHandle, APlayerController* PC, UItem_CardAugment* CardAugment, int32 Index);


protected:
#if WITH_EDITOR
	virtual void PostLoad() override;
	void RecalculateDominantHue();
#endif
};