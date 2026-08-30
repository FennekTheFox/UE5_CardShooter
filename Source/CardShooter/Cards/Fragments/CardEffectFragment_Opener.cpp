#include "CardEffectFragment_Opener.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Opener::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("OpenerAttrName", "Opener");
}

FText UCardEffectFragment_Opener::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	return LOCTEXT("OpenerAttrDesc", "This card will always be shuffled to the front of the deck.");
}

UCardEffectFragment_Opener::UCardEffectFragment_Opener()
{
	AttributeName = LOCTEXT("CardAttributes_Name_Opener", "Opener");
}

#undef LOCTEXT_NAMESPACE

