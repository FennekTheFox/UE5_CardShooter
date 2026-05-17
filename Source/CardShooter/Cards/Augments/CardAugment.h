#pragma once

#include "GameplayTagContainer.h"
#include "Cards/Fragments/CardExecution_AdditionalData.h"

#include "CardAugment.generated.h"

#define MAX_AUGMENT_SOCKET_COUNT 3


UCLASS(BlueprintType, Abstract)
class UCardAugment : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AugmentTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* DefaultAugmentTexutre;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor DefaultTint = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FLinearColor, FGameplayTagQuery> TextureTintMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxLevel;

	UFUNCTION(BlueprintPure)
	void GetAugmentStyleData(const FCardAugmentData& Data, class UTexture2D*& Texture, FLinearColor& Tint);
};