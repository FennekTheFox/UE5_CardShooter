#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"

#include "Validator_CardParamsHaveScalingGroup.generated.h"

class UCardData;
class UCardEffectFragment_Execution;

UCLASS()
class UValidator_CardParamsHaveScalingGroup : public UEditorValidatorBase
{
    GENERATED_BODY()

public:

    virtual bool CanValidateAsset_Implementation(
        const FAssetData& InAssetData,
        UObject* InObject,
        FDataValidationContext& InContext
    ) const override;

    EDataValidationResult ValidateLoadedAsset_Implementation(
        const FAssetData& InAssetData,
        UObject* InAsset,
        FDataValidationContext& Context) override;

private:

    static void ValidateExecutionRecursive(
        const UCardData* CardData,
        const UCardEffectFragment_Execution* Execution,
        const FString& Path,
        FDataValidationContext& Context,
        bool& bIsValid
    );
};