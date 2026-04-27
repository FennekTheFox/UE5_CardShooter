#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardAttributeFragments_DeckLimit.generated.h"

class UCardData;

UCLASS(BlueprintType)
class UDeckLimitationViolation : public UObject 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	virtual FText GetFailureReasonText() { return FText::GetEmpty(); };
};


UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_DeckLimitation : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:
	virtual bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) {return false;};
};

UCLASS(BlueprintType)
class UDeckLimitationViolation_KeystoneLimit : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};

//Keystone attributes limit the number of Keystone Attribute cards per deck, lowest number dominates
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Keystone"))
class UCardEffectFragment_Keystone : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 KeystoneLimit = 3;

	bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;
};

UCLASS(BlueprintType)
class UCardArchetype : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
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

	bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;
};

UCLASS(BlueprintType)
class UDeckLimitationViolation_Gregarious : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};

//obscure cards can only be added to decks of a minimum certain size
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Gregarious"))
class UCardEffectFragment_Gregarious : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MinimumDeckSize = 10;

	bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;
};

UCLASS(BlueprintType)
class UDeckLimitationViolation_Reclusive : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};


//prominent cards can only be added to decks of a minimum certain size
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Reclusive"))
class UCardEffectFragment_Reclusive : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MaximumDeckSize = 10;

	bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;
};



UCLASS(BlueprintType)
class UDeckLimitationViolation_Limit : public UDeckLimitationViolation
{
	GENERATED_BODY()

public:
	FText GetFailureReasonText() override;
};


//prominent cards can only be added to decks of a minimum certain size
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Deck Limit: Limited"))
class UCardEffectFragment_Limited : public UCardEffectFragment_DeckLimitation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Limit = 3;

	bool Verify(TArray<UCardData*> Deck, TArray<UDeckLimitationViolation*>& FailureReasons) override;
};