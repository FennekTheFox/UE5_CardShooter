#include "CardEffectFragment_Persistent.h"


#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Persistent::GetName_Implementation()
{
	return LOCTEXT("PersistentAttrName", "Persistent");
}

#undef LOCTEXT_NAMESPACE