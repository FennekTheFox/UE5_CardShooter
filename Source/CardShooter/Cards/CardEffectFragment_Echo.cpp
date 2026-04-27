#include "CardEffectFragment_Echo.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText UCardEffectFragment_Echo::GetName_Implementation()
{
	return FText::Format(LOCTEXT("EchoAttrName", "Echo({0}, {1})"), EchoCount, EchoDelay);
}


#undef LOCTEXT_NAMESPACE