#include "CardEffectFragment_DebugPrint.h"


#define LOCTEXT_NAMESPACE "CardEffects"


void UCardEffectFragment_Execution_DebugPrint::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	GEngine->AddOnScreenDebugMessage(-1, DisplayDuration, DisplayColor, Message);
}

FText UCardEffectFragment_Execution_DebugPrint::GetLocalizableDescription_Implementation()
{
	return FText::Format(
		LOCTEXT("DebugPrintName", "Prints Debug Message: {0}"),
		FText::FromString(Message)
	);
}

#undef LOCTEXT_NAMESPACE