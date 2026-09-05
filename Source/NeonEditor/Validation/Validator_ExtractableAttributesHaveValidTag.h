#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "Validator_ExtractableAttributesHaveValidTag.generated.h"

UCLASS()
class UValidator_ExtractableAttributesHaveValidTag : public UEditorValidatorBase
{
    GENERATED_BODY()

public:
    UValidator_ExtractableAttributesHaveValidTag();

protected:
    virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject, FDataValidationContext& InContext) const override;
    virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& InContext) override;
};