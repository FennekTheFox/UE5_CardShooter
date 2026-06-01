#pragma once

#include "CardAttributeFragment_Base.h"
#include "NativeGameplayTags.h"

#include "CardEffectFragment_Utility.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UtilityPrimaryCooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_UtilitySecondaryCooldown);

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Utility"))
class UCardEffectFragment_Utility : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

	UCardEffectFragment_Utility();

public:
	void PostInitProperties() override;

	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	FText GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment

private:
	UPROPERTY()
	TSubclassOf<UCardParameter> CooldownParamClass;
};