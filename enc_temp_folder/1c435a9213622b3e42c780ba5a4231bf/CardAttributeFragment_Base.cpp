#include "CardAttributeFragment_Base.h"

#include "CardExecution_AdditionalData.h"
#include "Cards/Augments/CardAugment.h"
#include "Cards/CardData.h"

#define LOCTEXT_NAMESPACE "CardEffectFragment"

bool UCardEffectFragment_Base::GetShouldShowInDescription()
{
	return bShowInDescription;
}

FText UCardEffectFragment_Base::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::GetEmpty();
}

FText UCardEffectFragment_Base::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return FText::GetEmpty();
}

FText UCardEffectFragment_Base::GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const
{
	return FText::GetEmpty();
}


void UCardEffectFragment_Attribute::GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity) const
{
	//create temporary proxy exec data
	FCardExecution_AdditionalData tempAddData;

	for (const UCardParameter* Param : Parameters)
	{
		if (!Param)
		{
			continue;
		}

		tempAddData.SetByCallerMetaDataTags.Add(Param->ParameterTag);
		tempAddData.SetByCallerMetaDataValues.Add(Param->MetaData);
	}

	//apply the card augments
	TArray<UItem_CardAugment*> Augments;
	UCardData::GetAugments(CardIdentity, Augments);
	for (UItem_CardAugment* AugmentItem : Augments)
	{
		if (!AugmentItem)
		{
			continue;
		}
		FCardAugmentData AugmentData = AugmentItem->AugmentData;
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

	for (const UCardParameter* Param : Parameters)
	{
		if (!Param)
		{
			continue;
		}
		PostModificationData.SetByCallerMetaDataTags.Add(Param->ParameterTag);
		PostModificationData.SetByCallerMetaDataValues.Add(Param->MetaData);
	}

	int32 ParamIndex = PostModificationData.SetByCallerMetaDataTags.Find(ParamTag);

	if (ParamIndex == INDEX_NONE)
	{
		return false;
	}

	//apply the card augments
	TArray<UItem_CardAugment*> Augments;
	UCardData::GetAugments(CardIdentity, Augments);
	for (UItem_CardAugment* AugmentItem : Augments)
	{
		if (!AugmentItem)
		{
			continue;
		}

		FCardAugmentData AugmentData = AugmentItem->AugmentData;
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

void UCardParameter::GetHoverText_Implementation(FText& OutText) const
{
	OutText = LOCTEXT("CardParameterHoverFormat", "<hover id=\"{0}\" relatedExec=\"{1}\" cardidentityhandle=\"{2}\" style=\"{3}\">{4}</>");
}

void UCardParameter_ScalingGroupParameter::GetHoverContentTitle_Implementation(FText& OutText) const
{
	OutText = FText::Format(LOCTEXT("CardParam_SGParam_Title", "Card Parameter: {0}"), MetaData.ParameterName);
}

void UCardParameter_ScalingGroupParameter::GetHoverContent_Implementation(FText& OutText) const
{
	OutText = FText::GetEmpty();

	TArray<FText> Segments;
	Segments.Add(FText::Format(LOCTEXT("CardParam_BaseValue", "Base Value: {0}"), MetaData.Magnitude));
	if(MetaData.MagnitudeRange.GetLowerBound().GetValue() != -BIG_NUMBER) Segments.Add(FText::Format(LOCTEXT("CardParam_LowerBound", "Lower Bound: {0}"), MetaData.MagnitudeRange.GetLowerBound().GetValue()));
	if(MetaData.MagnitudeRange.GetUpperBound().GetValue() != BIG_NUMBER) Segments.Add(FText::Format(LOCTEXT("CardParam_UpperBound", "Upper Bound: {0}"), MetaData.MagnitudeRange.GetUpperBound().GetValue()));

	const FText* SGName = ScalingGroupNames.Find(MetaData.ScalingGroup);
	if (ensure(SGName))
	{
		Segments.Add(FText::Format(LOCTEXT("CardParam_ScalingGroup", "Parameter Type: {0}"), *SGName));
	}

	OutText = FText::Join(FText::FromString("\n"), Segments);
}

FText UCardParameter_ScalingGroupParameter::GetStyle(const FSetByCallerMetaData& InMetaData)
{
	return FText::FromString("HoverContent_Param");
}

void UCardParameter_DamageType::GetHoverContentTitle_Implementation(FText& OutText) const
{
	OutText = FText::Format(LOCTEXT("CardParam_DamageType_Title", "Damage Type: {0}"), MetaData.GetDisplayText());
}

void UCardParameter_DamageType::GetHoverContent_Implementation(FText& OutText) const
{
	OutText = FText::Format(LOCTEXT("CardParam_DamageType_Desc", "Default Type: {0}"), MetaData.GetDisplayText());
}

FText UCardParameter_DamageType::GetStyle(const FSetByCallerMetaData& InMetaData)
{
	const float& Magnitude = MetaData.Magnitude;

	TSubclassOf<UNeonDamageType> DamageTypeClass = nullptr;

	for(auto&& KVPair : DamageTypeMap)
	{
		if (FMath::IsNearlyEqual(Magnitude, KVPair.Key, 0.05))
		{
			DamageTypeClass = KVPair.Value;
			break;
		}
	}

	if (ensure(DamageTypeClass))
	{
		UNeonDamageType* CDO = DamageTypeClass->GetDefaultObject<UNeonDamageType>();

		if (ensure(CDO))
		{
			return CDO->RichStyle;
		}
	}

	return FText::FromString("HoverContent_Param");
}

#undef LOCTEXT_NAMESPACE
