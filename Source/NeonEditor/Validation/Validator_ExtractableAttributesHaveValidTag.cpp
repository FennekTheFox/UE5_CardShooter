#include "Validator_ExtractableAttributesHaveValidTag.h"
#include "Cards/Fragments/CardAttributeFragment_Base.h" // Replace with your actual fragment header path
#include "Misc/DataValidation.h"

UValidator_ExtractableAttributesHaveValidTag::UValidator_ExtractableAttributesHaveValidTag()
{
    bIsEnabled = true;
}

bool UValidator_ExtractableAttributesHaveValidTag::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject, FDataValidationContext& InContext) const
{
    return InObject && InObject->IsA<UCardEffectFragment_Attribute>();
}

EDataValidationResult UValidator_ExtractableAttributesHaveValidTag::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& InContext)
{
    UCardEffectFragment_Attribute* Fragment = Cast<UCardEffectFragment_Attribute>(InAsset);
    if (!Fragment)
    {
        return EDataValidationResult::NotValidated;
    }

    EDataValidationResult Result = EDataValidationResult::Valid;

    if (Fragment->GetCanBeExtracted() && !Fragment->AttributeTag.IsValid())
    {
        AssetFails(
            InAsset,
            FText::FromString(TEXT("bCanBeExtracted is true, but AttributeTag is not set or invalid!"))
        );
        Result = EDataValidationResult::Invalid;
    }

    if (Result == EDataValidationResult::Valid)
    {
        AssetPasses(InAsset);
    }

    return Result;
}