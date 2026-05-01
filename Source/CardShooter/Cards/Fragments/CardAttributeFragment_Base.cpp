#include "CardAttributeFragment_Base.h"

bool UCardEffectFragment_Base::GetShouldShowInDescription()
{
	return bShowInDescription;
}

FText UCardEffectFragment_Base::GetLocalizableName_Implementation()
{
	return FText::GetEmpty();
}

FText UCardEffectFragment_Base::GetLocalizableDescription_Implementation()
{
	return FText::GetEmpty();
}
