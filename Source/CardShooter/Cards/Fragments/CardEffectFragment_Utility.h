#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Utility.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Utility"))
class UCardEffectFragment_Utility : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment

public:
	//The cooldown length applied when this card is used as a utility card and its primary function is used
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PrimaryUtilityCooldown = 5.f;

	//The cooldown length applied when this card is used as a utility card and its primary function is used
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SecondaryUtilityCooldown = 5.f;
};