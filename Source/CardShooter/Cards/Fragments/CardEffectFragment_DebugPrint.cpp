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


UCardExecutionInstance* UCardEffectFragment_Execution_DebugPrint::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance_DebugPrint>(this);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
}

FText UCardEffectFragment_Execution_DebugPrint::GetLocalizableDescription_Implementation()
{
	return FText::Format(
		LOCTEXT("DebugPrintName", "Prints Debug Message: {0}"),
		FText::FromString(Message)
	);
}

#undef LOCTEXT_NAMESPACE
