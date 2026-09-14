#include "NeonDamageType.h"

#include "Cards/Fragments/CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "DamageTypes"

void UNeonDamageType::GetHoverText_Implementation(FText& OutText) const
{
	OutText = DamageTypeName;
}

void UNeonDamageType::GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec) const
{
	OutText = FText::Format(LOCTEXT("DamageTypeTitleFormat", "Damage Type: {0}"),
		DamageTypeName);
}

void UNeonDamageType::GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec) const
{
	OutText = DamageTypeDescription;
}

void UNeonDamageType::GetFormattingArgs(const UCardEffectFragment_Execution_WithParameters* RelatedExec, FFormatNamedArguments& FormattingArguments) const
{
	UCardEffectFragment_Execution_WithParameters::AppendFormattingArgsFromHoverContextObjects(RelatedExec, HoverContextObjects, FormattingArguments);
}

#undef LOCTEXT_NAMESPACE