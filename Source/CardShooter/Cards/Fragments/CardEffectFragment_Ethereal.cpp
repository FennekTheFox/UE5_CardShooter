#include "CardEffectFragment_Ethereal.h"


#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Ethereal::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("EtherealAttrName", "Ethereal");
}

FText UCardEffectFragment_Ethereal::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return LOCTEXT("EtherealAttrDesc", "Once used, this card is removed from your deck.");
}

UCardEffectFragment_Ethereal::UCardEffectFragment_Ethereal()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Ethereal", "Ethereal");
}

#undef LOCTEXT_NAMESPACE