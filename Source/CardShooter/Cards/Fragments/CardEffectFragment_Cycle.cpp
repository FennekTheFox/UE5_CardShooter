#include "CardEffectFragment_Cycle.h"

#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardEffects"



void UCardExecutionInstance_Cycle::ProcessExecutionInstance()
{
	UCardEffectFragment_Execution_Cycle* AsCycle = Cast<UCardEffectFragment_Execution_Cycle>(Exec);

	if (!ensure(AsCycle))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	if (AsCycle->Executions.IsValidIndex(Index))
	{
		UCardEffectFragment_Execution* NextExec = AsCycle->Executions[Index];

		if (ensure(Exec))
		{
			UCardExecutionInstance* Inst = NextExec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData, Exec);
			RunningSubInstances.Add(Inst);
			FDelegateHandle OnExecutionCompletedHandle;

			TWeakObjectPtr<APawn> WeakExecutingPawn = ExecutingPawn;
			OnExecutionCompletedHandle = Inst->OnExecutionCompleted.AddLambda(
				[this, Inst, WeakExecutingPawn, AsCycle]
				(APawn* CallbackPawn, const FCardExecution_AdditionalData& CallbackData, UCardEffectFragment_Execution* CallbackExec) {
					if (WeakExecutingPawn.IsValid())
					{
						Index = (Index + 1) % AsCycle->Executions.Num();
						ProcessExecutionInstance();
					}

					RunningSubInstances.Remove(Inst);
				});
		}
	}
}

FText UCardEffectFragment_Execution_Cycle::GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const
{
	TArray<FText> ExecDescs;
	ExecDescs.Add(LOCTEXT("CycleName", "Cycle"));

	int32 Count = 1;
	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		if (IsValid(Exec))
		{
			FText Base = Exec->GetLocalizableDescription(CardIdentity);
			FText SequenceFormat = LOCTEXT("CycleFormat", "{0}: {1}");
			ExecDescs.Add(FText::Format(SequenceFormat, Count, Base));
			Count++;
		}
	}

	return FText::Join(FText::FromString("\n"), ExecDescs);
}

UCardEffectFragment_Execution_Cycle::UCardEffectFragment_Execution_Cycle()
{
	ExecInstanceClass = UCardExecutionInstance_Cycle::StaticClass();
}

#undef LOCTEXT_NAMESPACE
