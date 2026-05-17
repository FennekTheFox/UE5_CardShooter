#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Ethereal.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Ethereal"))
class UCardEffectFragment_Ethereal : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	//~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	//~UCardEffectFragment
};