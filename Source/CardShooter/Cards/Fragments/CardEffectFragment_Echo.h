#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Echo.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Echo"))
class UCardEffectFragment_Echo : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 EchoCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float EchoDelay;
};