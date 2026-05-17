#include "CardEffectFragment_DebugPrint.h"


#define LOCTEXT_NAMESPACE "CardEffects"

void UCardExecutionInstance_DebugPrint::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_DebugPrint* AsDebugPrint = Cast<UCardEffectFragment_Execution_DebugPrint>(Exec);

	if (!ensure(AsDebugPrint))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 
		AsDebugPrint->DisplayDuration,
		AsDebugPrint->DisplayColor, 
		AsDebugPrint->Message);

	OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
}

FText UCardEffectFragment_Execution_DebugPrint::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity)
{
	return FText::Format(
		LOCTEXT("DebugPrintName", "Prints Debug Message: {0}"),
		FText::FromString(Message)
	);
}

UCardEffectFragment_Execution_DebugPrint::UCardEffectFragment_Execution_DebugPrint()
{
	ExecInstanceClass = UCardExecutionInstance_DebugPrint::StaticClass();
}

#undef LOCTEXT_NAMESPACE
