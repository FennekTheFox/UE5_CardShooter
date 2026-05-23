// Validator_CardDescUsesAllParameters.cpp

#include "Validator_CardDescUsesAllParameters.h"

#include "Cards/CardData.h"
#include "Cards/CardIdentity.h"

bool UValidator_CardDescUsesAllParameters::CanValidateAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InAsset,
	FDataValidationContext& Context) const
{
	return InAsset && InAsset->IsA<UCardData>();
}

EDataValidationResult UValidator_CardDescUsesAllParameters::ValidateLoadedAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InAsset, 
	FDataValidationContext& Context)

{
	UCardData* CardData = Cast<UCardData>(InAsset);
	if (!CardData)
	{
		return EDataValidationResult::NotValidated;
	}

	bool bValid = true;

	TArray<FGameplayTag> UsedParameters;
	CardData->GetAllUsedParameters(UsedParameters);

	FCardIdentity Identity;
	CardData->GetIdentity(Identity);

	const FString Description = CardData->GetDescription(Identity).ToString();

	for (const FGameplayTag& ParameterTag : UsedParameters)
	{
		if (!ParameterTag.IsValid())
		{
			continue;
		}

		FString Discard;
		FString ParameterKey;

		if (!ParameterTag.ToString().Split(
			TEXT("."),
			&Discard,
			&ParameterKey,
			ESearchCase::IgnoreCase,
			ESearchDir::FromEnd))
		{
			AssetFails(
				InAsset,
				FText::FromString(FString::Printf(
					TEXT("Invalid parameter tag '%s'."),
					*ParameterTag.ToString()
				))
			);

			bValid = false;
			continue;
		}

		const FString RequiredToken =
			FString::Printf(TEXT("{%sMagnitude}"), *ParameterKey);

		if (!Description.Contains(RequiredToken, ESearchCase::CaseSensitive))
		{
			AssetFails(
				InAsset,
				FText::FromString(FString::Printf(
					TEXT("Missing description token '%s' for parameter '%s'."),
					*RequiredToken,
					*ParameterTag.ToString()
				))
			);

			bValid = false;
		}
	}

	return bValid
		? EDataValidationResult::Valid
		: EDataValidationResult::Invalid;
}