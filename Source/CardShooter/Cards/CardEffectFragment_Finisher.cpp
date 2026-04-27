#include "CardEffectFragment_Finisher.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Finisher::GetName_Implementation()
{
	return LOCTEXT("FinisherAttrName", "Finisher");
}

#undef LOCTEXT_NAMESPACE