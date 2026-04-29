#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Combo.generated.h"

//Allows for the repeated execution of the card
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Combo"))
class UCardEffectFragment_Combo : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 ComboCount = 2;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ComboWindow = 3.f;

	//~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation() override;
	//~UCardEffectFragment
};