#include "CardAttributeFragments_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	ExecuteCardEffect(ExecutingPawn, AdditionalData);
}

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Delayed_Implementation(APawn* ExecutingPawn, float Delay /*= 1.f*/, int32 IterationCount /*= 1*/)
{
	FTimerHandle Handle;
	TWeakObjectPtr<APawn> WeakPawn = ExecutingPawn;
	TWeakObjectPtr<UCardEffectFragment_Execution> WeakThis = this;

	ExecutingPawn->GetWorldTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([WeakThis, WeakPawn, IterationCount]()
			{
				if (WeakPawn.IsValid() || WeakThis.IsValid())
				{
					FCardExecution_AdditionalData Data;
					Data.IterationCount = IterationCount;

					WeakThis->ExecuteCardEffect(WeakPawn.Get(), Data);
				}
			}),
		Delay, false);
}

void UCardEffectFragment_Execution::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData /*= FCardExecution_AdditionalData()*/)
{

}

void UCardEffectFragment_Execution_GameplayAbility::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	Super::ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		return;
	}

	FGameplayAbilitySpec AbilitySpec(AbilityToExecute.Ability);
	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);

	//This relies on the ability 100% getting executed, else it wont get removed properly
	TSharedPtr<FDelegateHandle> OnEndedHandle = MakeShared<FDelegateHandle>();
	TSharedPtr<FDelegateHandle> OnFailedHandle = MakeShared<FDelegateHandle>();
	*OnEndedHandle = ASC->OnAbilityEnded.AddLambda([ASC, Handle, AbilitySpec, OnEndedHandle, OnFailedHandle](const FAbilityEndedData& Data) {
		if (Data.AbilityThatEnded == AbilitySpec.Ability)
		{
			ASC->ClearAbility(Handle);

			if (OnEndedHandle->IsValid())
			{
				ASC->OnAbilityEnded.Remove(*OnEndedHandle);
			}
			if (OnFailedHandle->IsValid())
			{
				ASC->AbilityFailedCallbacks.Remove(*OnFailedHandle);
			}
		}
		}
	);

	*OnFailedHandle = ASC->AbilityFailedCallbacks.AddLambda([ASC, Handle, AbilitySpec, OnEndedHandle, OnFailedHandle](const UGameplayAbility* Ability, const FGameplayTagContainer&) {
		if (Ability == AbilitySpec.Ability)
		{
			ASC->ClearAbility(Handle);

			if (OnEndedHandle->IsValid())
			{
				ASC->OnAbilityEnded.Remove(*OnEndedHandle);
			}
			if (OnFailedHandle->IsValid())
			{
				ASC->AbilityFailedCallbacks.Remove(*OnFailedHandle);
			}
		}
		}
	);
}

void UCardEffectFragment_Execution_DebugPrint::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, FCardExecution_AdditionalData AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	GEngine->AddOnScreenDebugMessage(-1, DisplayDuration, DisplayColor, Message);
}
