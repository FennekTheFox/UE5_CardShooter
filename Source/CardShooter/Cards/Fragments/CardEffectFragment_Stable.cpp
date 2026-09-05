#include "CardEffectFragment_Stable.h"

#define LOCTEXT_NAMESPACE "CardEffects"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Attribute_Stable, "Gameplay.Core.Cards.Attribute.Stable")

FText UCardEffectFragment_Stable::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("StableAttrName", "Stable");
}

FText UCardEffectFragment_Stable::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return LOCTEXT("StableAttrDesc", "A stable card will not be discarded with the other cards. \
		Holding on to a stable card will also not prevent shuffling an empty deck.");
}

UCardEffectFragment_Stable::UCardEffectFragment_Stable()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Stable", "Stable");
	bCanBeExtracted = true;
	AttributeTag = TAG_Card_Attribute_Stable;
}

#undef LOCTEXT_NAMESPACE