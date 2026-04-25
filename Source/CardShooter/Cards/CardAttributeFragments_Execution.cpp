#include "CardAttributeFragments_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn)
{

}

void UCardEffectFragment_Execution_GameplayAbility::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn)
{
	Super::Server_ExecuteCardEffect_Implementation(ExecutingPawn);

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

void UCardEffectFragment_Execution_DebugPrint::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn)
{
	GEngine->AddOnScreenDebugMessage(-1, DisplayDuration, DisplayColor, Message);
}
