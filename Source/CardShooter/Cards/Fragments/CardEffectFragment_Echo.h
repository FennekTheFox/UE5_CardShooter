#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Echo.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Echo"))
class UCardEffectFragment_Echo : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

	UCardEffectFragment_Echo();

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 EchoCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float EchoDelay;
};