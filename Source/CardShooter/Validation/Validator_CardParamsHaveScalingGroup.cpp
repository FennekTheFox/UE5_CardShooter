#include "Validator_CardParamsHaveScalingGroup.h"

#include "Cards/CardData.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"
#include "Misc/DataValidation.h"

bool UValidator_CardParamsHaveScalingGroup::CanValidateAsset_Implementation(
    const FAssetData& InAssetData,
    UObject* InObject,
    FDataValidationContext& InContext
) const
{
    return InObject && InObject->IsA<UCardData>();
}

EDataValidationResult UValidator_CardParamsHaveScalingGroup::ValidateLoadedAsset_Implementation(
    const FAssetData& InAssetData,
    UObject* InAsset,
    FDataValidationContext& Context
)
{
    UCardData* CardData = Cast<UCardData>(InAsset);
    if (!CardData)
    {
        return EDataValidationResult::NotValidated;
    }

    bool bIsValid = true;

    ValidateExecutionRecursive(
        CardData,
        CardData->PrimaryExecution,
        TEXT("PrimaryExecution"),
        Context,
        bIsValid
    );

    ValidateExecutionRecursive(
        CardData,
        CardData->SecondaryExecution,
        TEXT("SecondaryExecution"),
        Context,
        bIsValid
    );

    return bIsValid
        ? EDataValidationResult::Valid
        : EDataValidationResult::Invalid;
}

void UValidator_CardParamsHaveScalingGroup::ValidateExecutionRecursive(
    const UCardData* CardData,
    const UCardEffectFragment_Execution* Execution,
    const FString& Path,
    FDataValidationContext& Context,
    bool& bIsValid
)
{
    if (!Execution)
    {
        return;
    }

    if (const UCardEffectFragment_Execution_Compound* Compound =
        Cast<UCardEffectFragment_Execution_Compound>(Execution))
    {
        for (int32 Index = 0; Index < Compound->Executions.Num(); ++Index)
        {
            ValidateExecutionRecursive(
                CardData,
                Compound->Executions[Index],
                FString::Printf(TEXT("%s.Executions[%d]"), *Path, Index),
                Context,
                bIsValid
            );
        }

        return;
    }

    if (const UCardEffectFragment_Execution_WithParameters* WithParams =
        Cast<UCardEffectFragment_Execution_WithParameters>(Execution))
    {
        for (const auto& KVPair : WithParams->SetByCallerMagnitudes)
        {
            const FGameplayTag& Tag = KVPair.Key;
            const FSetByCallerMetaData& MetaData = KVPair.Value;
            const FString SGAsString = MetaData.ScalingGroup.ToString();

            if (!MetaData.ScalingGroup.IsValid() || !SGAsString.StartsWith(TEXT("Gameplay.Effect.ScalingGroups.")))
            {
                bIsValid = false;

                Context.AddError(
                    FText::Format(
                        NSLOCTEXT(
                            "CardValidation",
                            "MissingScalingGroup",
                            "{0}: {1} → SetByCaller '{2}' has no or invalid ScalingGroup"
                        ),
                        FText::FromString(GetNameSafe(CardData)),
                        FText::FromString(Path),
                        FText::FromName(Tag.GetTagName())
                    )
                );
            }
        }
    }
}