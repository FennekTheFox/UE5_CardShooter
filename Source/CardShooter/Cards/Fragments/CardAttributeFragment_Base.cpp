#include "CardAttributeFragment_Base.h"

#include "CardExecution_AdditionalData.h"
#include "Cards/Augments/CardAugment.h"

bool UCardEffectFragment_Base::GetShouldShowInDescription()
{
	return bShowInDescription;
}

FText UCardEffectFragment_Base::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::GetEmpty();
}

FText UCardEffectFragment_Base::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::GetEmpty();
}

FText UCardEffectFragment_Base::GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const
{
	return FText::GetEmpty();
}

void UCardEffectFragment_Attribute::GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity)
{
	//create temporary proxy exec data
	FCardExecution_AdditionalData tempAddData;

	for (auto&& KVPair : SetByCallerMagnitudes)
	{
		tempAddData.SetByCallerMetaDataTags.Add(KVPair.Key);
		tempAddData.SetByCallerMetaDataValues.Add(KVPair.Value);
	}

	//apply the card augments
	for (FCardAugmentData AugmentData : CardIdentity.Augments)
	{
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =
			AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
		{
			AugmentCDO = AugmentClass->GetDefaultObject<UCardAugment>();
		}

		if (AugmentCDO)
		{
			AugmentCDO->ApplyAugment(tempAddData, AugmentData);
		}
	}

	//Iterate over all SetByCaller magnitudes
	for (int32 i = 0; i < tempAddData.SetByCallerMetaDataTags.Num(); i++)
	{
		const FSetByCallerMetaData& MetaData = tempAddData.SetByCallerMetaDataValues[i];

		FString TagString = tempAddData.SetByCallerMetaDataTags[i].ToString();

		FString Left;
		FString Right;

		FString LastPart = TagString;

		if (TagString.Split(
			TEXT("."),
			&Left,
			&Right,
			ESearchCase::IgnoreCase,
			ESearchDir::FromEnd))
		{
			LastPart = Right;
		}

		Args.Add(
			LastPart + "Magnitude",
			MetaData.GetDisplayText()
		);

		Args.Add(
			LastPart + "Parameter",
			MetaData.ParameterName
		);
	}
}

bool UCardEffectFragment_Attribute::GetParameterValue(FGameplayTag ParamTag, float& OutValue, const FCardIdentity& CardIdentity) const
{
	//Empty data, we just care about our few values
	FCardExecution_AdditionalData PostModificationData;

	for (auto&& KVPair : SetByCallerMagnitudes)
	{
		PostModificationData.SetByCallerMetaDataTags.Add(KVPair.Key);
		PostModificationData.SetByCallerMetaDataValues.Add(KVPair.Value);
	}

	int32 ParamIndex = PostModificationData.SetByCallerMetaDataTags.Find(ParamTag);

	if (ParamIndex == INDEX_NONE)
	{
		return false;
	}

	for (FCardAugmentData AugmentData : CardIdentity.Augments)
	{
		UCardAugment* AugmentCDO = nullptr;

		if (UClass* AugmentClass =
			AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
		{
			AugmentCDO = AugmentClass->GetDefaultObject<UCardAugment>();
		}

		if (AugmentCDO)
		{
			AugmentCDO->ApplyAugment(PostModificationData, AugmentData);
		}
	}

	OutValue = PostModificationData.SetByCallerMetaDataValues[ParamIndex].Magnitude;

	return true;
}
