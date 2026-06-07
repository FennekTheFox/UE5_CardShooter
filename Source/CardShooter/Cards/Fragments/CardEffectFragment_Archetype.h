#pragma once

#include "Engine/DataAsset.h"
#include "CardAttributeFragment_DeckLimit.h"

#include "CardEffectFragment_Archetype.generated.h"

UCLASS(BlueprintType)
class UCardArchetype : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ArchetypeName;
};

UCLASS(BlueprintType)
class UDeckLimitationViolation_Archetype : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};

//Archetype attributes limit the deck to use one archetype exclusively.
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Archetype"))
class UCardEffectFragment_Archetype : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UCardArchetype> Archetype;

	bool Verify(const FCardDeck_DataOnly& Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;

	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};