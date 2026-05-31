#include "Validator_CardDescUsesAllParameters.h"

#include "Cards/CardData.h"
#include "Cards/CardIdentity.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"
#include "Cards/Fragments/CardEffectFragment_GameplayAbility.h"
#include "Gameplay/GameplayEffect_CardExecution.h"
#include "UI/RichTextHoverDecorator.h"

//UValidator_CardDescUsesAllParameters::UValidator_CardDescUsesAllParameters()
//{
//	bIsEnabled = true;
//}

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

	TArray<FGameplayTag> UsedParameters;
	CardData->GetAllUsedParameters(UsedParameters);
	UsedParameters.RemoveAll([this](const FGameplayTag& Tag)
		{
			return Exceptions.Contains(Tag);
		});

	bool bValid = true;

	FCardIdentity Identity;
	CardData->GetIdentity(Identity);

	ValidateExecutionRecursive(CardData->PrimaryExecution, Identity, UsedParameters, InAsset);
	ValidateExecutionRecursive(CardData->SecondaryExecution, Identity, UsedParameters, InAsset);

	for (const UCardEffectFragment_Attribute* Attr : CardData->CardAttributes)
	{
		for (int32 i = 0; i < UsedParameters.Num(); i++)
		{
			const FGameplayTag ParameterTag = UsedParameters[i];

			if (!ParameterTag.IsValid())
			{
				continue;
			}

			FString Unused;
			FString ParameterKey;

			if (!ParameterTag.ToString().Split(
				TEXT("."),
				&Unused,
				&ParameterKey,
				ESearchCase::IgnoreCase,
				ESearchDir::FromEnd))
			{
				AssetFails(
					InAsset,
					FText::FromString(FString::Printf(
						TEXT("%s: Invalid parameter tag '%s'."),
						*Attr->GetName(),
						*ParameterTag.ToString()
					))
				);

				continue;
			}

			const FString RequiredToken =
				FString::Printf(TEXT("{%sMagnitude}"), *ParameterKey);

			const FString AttrDesc = Attr->GetRawLocalizableDescription(Identity).ToString();

			if (AttrDesc.Contains(RequiredToken, ESearchCase::CaseSensitive))
			{
				UsedParameters.Remove(ParameterTag);
				i--;

				continue;
			}
		}

	}

	for(const FGameplayTag& MissingParameter : UsedParameters)
	{
		FString Unused;
		FString ParameterKey;

		if (!MissingParameter.ToString().Split(
			TEXT("."),
			&Unused,
			&ParameterKey,
			ESearchCase::IgnoreCase,
			ESearchDir::FromEnd))
		{
			AssetFails(
				InAsset,
				FText::FromString(FString::Printf(
					TEXT("%s: Invalid parameter tag '%s'."),
					*CardData->GetName(),
					*MissingParameter.ToString()
				))
			);
			continue;
		}

		const FString RequiredToken =
			FString::Printf(TEXT("{%sMagnitude}"), *ParameterKey);

		AssetFails(
			InAsset,
			FText::FromString(FString::Printf(
				TEXT("%s: Missing token '%s' in raw localizable description for parameter '%s'."),
				*CardData->GetName(),
				*RequiredToken,
				*MissingParameter.ToString()
			))
		);
	}

	return UsedParameters.IsEmpty()
		? EDataValidationResult::Valid
		: EDataValidationResult::Invalid;
}


UValidator_CardDescUsesAllParameters::UValidator_CardDescUsesAllParameters()
{
	Exceptions.Add(FGameplayTag::RequestGameplayTag("Gameplay.Effect.Parameters.SecondaryUtilityCooldown"));
}

void UValidator_CardDescUsesAllParameters::ValidateExecutionRecursive(
	class UCardEffectFragment_Execution* Execution,
	const FCardIdentity& Identity,
	TArray<FGameplayTag>& UsedParameters,
	UObject* Asset)
{
	if (!Execution)
	{
		return;
	}

	if (UCardEffectFragment_Execution_Compound* Compound =
		Cast<UCardEffectFragment_Execution_Compound>(Execution))
	{
		for (UCardEffectFragment_Execution* ChildExecution : Compound->Executions)
		{
			ValidateExecutionRecursive(ChildExecution, Identity, UsedParameters, Asset);
		}

		return;
	}

	if (UCardEffectFragment_Execution_WithParameters* WithParameters =
		Cast<UCardEffectFragment_Execution_WithParameters>(Execution))
	{
		const FString ExecDesc =
			WithParameters->GetRawLocalizableDescription(Identity).ToString();


		UCardEffectFragment_ApplyEffectToOwner* AsCEF_AETO = Cast<UCardEffectFragment_ApplyEffectToOwner>(WithParameters);
		FString EffectDesc = "";
		if (AsCEF_AETO)
		{
			if (auto* AsGE_CE = Cast<UGameplayEffect_CardExecution>(AsCEF_AETO->GameplayEffect->GetDefaultObject()))
			{
				EffectDesc = AsGE_CE->EffectDesc.ToString();
			}
		}
		UCardEffectFragment_GameplayAbility* AsCEF_GA = Cast<UCardEffectFragment_GameplayAbility>(WithParameters);

		for (int32 i = 0; i < UsedParameters.Num(); i++)
		{
			const FGameplayTag ParameterTag = UsedParameters[i];

			if (!ParameterTag.IsValid())
			{
				continue;
			}

			FString Unused;
			FString ParameterKey;

			if (!ParameterTag.ToString().Split(
				TEXT("."),
				&Unused,
				&ParameterKey,
				ESearchCase::IgnoreCase,
				ESearchDir::FromEnd))
			{
				AssetFails(
					Asset,
					FText::FromString(FString::Printf(
						TEXT("%s: Invalid parameter tag '%s'."),
						*Execution->GetName(),
						*ParameterTag.ToString()
					))
				);

				continue;
			}

			const FString RequiredToken =
				FString::Printf(TEXT("{%sMagnitude}"), *ParameterKey);

			if (ExecDesc.Contains(RequiredToken, ESearchCase::CaseSensitive))
			{
				UsedParameters.Remove(ParameterTag);
				i--;

				continue;
			}

			if (AsCEF_AETO)
			{
				if (EffectDesc.Contains(RequiredToken, ESearchCase::CaseSensitive))
				{
					UsedParameters.Remove(ParameterTag);
					i--;
					continue;
				}
			}

			if (AsCEF_GA) 
			{
				for (auto&& KVPair : AsCEF_GA->HoverContextObjects)
				{
					UGameplayEffect_CardExecution* AsGE_CE = Cast<UGameplayEffect_CardExecution>(KVPair.Value->GetDefaultObject());

					if (AsGE_CE&& AsGE_CE->EffectDesc.ToString().Contains(RequiredToken, ESearchCase::CaseSensitive))
					{
						UsedParameters.Remove(ParameterTag);
						i--;
						continue;
					}
				}

				for (auto&& KVPair : AsCEF_GA->HoverContextObjects)
				{
					if (!ensure(KVPair.Value))
					{
						continue;
					}

					if (!KVPair.Value->ImplementsInterface(UHoverableContext::StaticClass()))
					{
						continue;
					}

					UObject* CDO = KVPair.Value->GetDefaultObject();

					if (!ensure(CDO))
					{
						continue;
					}

					FText RawHoverContent;

					IHoverableContext::Execute_GetHoverContent(CDO, RawHoverContent);

					if (RawHoverContent.ToString().Contains(RequiredToken, ESearchCase::CaseSensitive))
					{
						UsedParameters.Remove(ParameterTag);
						i--;
						continue;
					}
				}
			}
		}
	}
}