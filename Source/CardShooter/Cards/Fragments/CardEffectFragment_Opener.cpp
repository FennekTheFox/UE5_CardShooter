#include "CardEffectFragment_Opener.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Opener::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("OpenerAttrName", "Opener");
}

FText UCardEffectFragment_Opener::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("OpenerAttrDesc", "This card will always be shuffled to the front of the deck.");
}

#undef LOCTEXT_NAMESPACE

