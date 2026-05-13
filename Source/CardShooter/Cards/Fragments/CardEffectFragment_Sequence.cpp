#include "CardEffectFragment_Sequence.h"

#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardEffects"


void UCardExecutionInstance_Sequence::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_Sequence* AsSequence = Cast<UCardEffectFragment_Execution_Sequence>(Exec);

	if (!ensure(AsSequence))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	if (AsSequence->Executions.IsValidIndex(Index))
	{
		UCardEffectFragment_Execution* NextExec = AsSequence->Executions[Index];

		if (ensure(Exec))
		{
			UCardExecutionInstance* Inst = NextExec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);
			RunningSubInstances.Add(Inst);
			FDelegateHandle OnExecutionCompletedHandle;

			TWeakObjectPtr<APawn> WeakExecutingPawn = ExecutingPawn;
			OnExecutionCompletedHandle = Inst->OnExecutionCompleted.AddLambda(
				[this, WeakExecutingPawn]
				(APawn* CallbackPawn, const FCardExecution_AdditionalData& CallbackData, UCardEffectFragment_Execution* CallbackExec) {
						if (WeakExecutingPawn.IsValid())
						{
							Index++;
							ProcessExecutionInstance();
						}
				});
		}
	}
	else
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
	}
}

UCardExecutionInstance* UCardEffectFragment_Execution_Sequence::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance_Sequence>(this);
	NewInstance->StartExecutionInstance(ExecutingPawn, AdditionalData, this);
	return NewInstance;
}

FText UCardEffectFragment_Execution_Sequence::GetLocalizableDescription_Implementation()
{
	TArray<FText> ExecDescs;
	ExecDescs.Add(LOCTEXT("SequenceName", "Sequence"));

	int32 Count = 1;
	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		if (IsValid(Exec))
		{
			FText Base = Exec->GetLocalizableDescription();
			FText SequenceFormat = LOCTEXT("SequenceFormat", "{0}: {1}");
			ExecDescs.Add(FText::Format(SequenceFormat, Count, Base));
			Count++;
		}
	}

	return FText::Join(FText::FromString("\n"), ExecDescs);
}

#undef LOCTEXT_NAMESPACE
