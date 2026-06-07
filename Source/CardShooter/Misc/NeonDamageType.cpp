#include "NeonDamageType.h"

#define LOCTEXT_NAMESPACE "DamageTypes"

void UNeonDamageType::GetHoverText_Implementation(FText& OutText) const
{
	OutText = DamageTypeName;
}

void UNeonDamageType::GetHoverContentTitle_Implementation(FText& OutText) const
{
	OutText = FText::Format(LOCTEXT("DamageTypeTitleFormat", "Damage Type: {0}"),
		DamageTypeName);
}

void UNeonDamageType::GetHoverContent_Implementation(FText& OutText) const
{
	OutText = DamageTypeDescription;
}

#undef LOCTEXT_NAMESPACE