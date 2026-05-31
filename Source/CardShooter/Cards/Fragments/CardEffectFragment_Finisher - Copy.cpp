#include "CardEffectFragment_Stable.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Stable::GetLocalizableName_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("StableAttrName", "Stable");
}

FText UCardEffectFragment_Stable::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	return LOCTEXT("StableAttrDesc", "A stable card will not be discarded with the other cards. \
		Holding on to a stable card will also not prevent shuffling an empty deck.");
}

#undef LOCTEXT_NAMESPACE