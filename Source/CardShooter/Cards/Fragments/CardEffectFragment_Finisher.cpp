#include "CardEffectFragment_Finisher.h"

#define LOCTEXT_NAMESPACE "CardEffects"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Attribute_Finisher, "Gameplay.Core.Cards.Attribute.Finisher")

FText UCardEffectFragment_Finisher::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("FinisherAttrName", "Finisher");
}

FText UCardEffectFragment_Finisher::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return LOCTEXT("FinisherAttrDesc", "This card always shuffles to the back of your deck.");
}

UCardEffectFragment_Finisher::UCardEffectFragment_Finisher()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Finisher", "Finisher");
	bCanBeExtracted = true;
	AttributeTag = TAG_Card_Attribute_Finisher;
}

#undef LOCTEXT_NAMESPACE