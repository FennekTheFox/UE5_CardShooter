#include "CardData.h"

FText UCardEffectFragment_Base::GetFragmentLabel()
{
	return FText::FromString("Unassigned Label");
}

FText UCardEffectFragment_Root::GetFragmentDescription()
{
	return FText::FromString("Unassigned Description");
}

void UCardEffectFragment_Execution::Execute_Implementation()
{

}
