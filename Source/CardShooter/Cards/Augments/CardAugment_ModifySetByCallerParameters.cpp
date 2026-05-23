#include "CardAugment_ModifySetByCallerParameters.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Augment, "Gameplay.Core.Cards.Augments.ModifyParameter");

UCardAugment_ModifySetByCallerParameter::UCardAugment_ModifySetByCallerParameter()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> AugmentChipIconFinder(
		TEXT("/Game/Assets/Textures/Icons/T_Icon_AugmentChip_Default"));

	if (ensure(AugmentChipIconFinder.Succeeded()))
	{
		DefaultAugmentTexutre = AugmentChipIconFinder.Object;
	}

	AugmentTag = TAG_Card_Augment;
}

void UCardAugment_ModifySetByCallerParameter::ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const
{
	for (auto&& KVPair : AugmentData.AugmentParameters)
	{
		if (!ensure(KVPair.Value.VariantOption >= 0 && KVPair.Value.VariantOption <= 1))
		{
			continue;
		}

		const int32 MagnitudeIndex = DataToModify.SetByCallerMetaDataTags.Find(KVPair.Key);

		if (MagnitudeIndex == INDEX_NONE)
		{
			//this just means this attribute isn't used by the execution
			continue;
		}

		float& MagnitudePtr = DataToModify.SetByCallerMetaDataValues[MagnitudeIndex].Magnitude;

		switch ((EModifyOperationType)KVPair.Value.VariantOption)
		{
		case EModifyOperationType::Add: MagnitudePtr += KVPair.Value.Magnitude; break;
		case EModifyOperationType::Multiply: MagnitudePtr *= KVPair.Value.Magnitude; break;
		}
	}
}
