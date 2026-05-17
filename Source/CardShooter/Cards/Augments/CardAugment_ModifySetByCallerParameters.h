#pragma once

#include "CardAugment.h"
#include "NativeGameplayTags.h"

#include "CardAugment_ModifySetByCallerParameters.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Augment);

UENUM()
enum class EModifyOperationType : uint8
{
	Add = 0,
	Subtract = 1,
	Multiply = 2,
	Divide = 3,
};

UCLASS()
class UCardAugment_ModifySetByCallerParameter : public UCardAugment
{
	GENERATED_BODY()

	UCardAugment_ModifySetByCallerParameter();

public:
	void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const override;
};