#include "CardEffectFragment_GameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/CardExecutionAbility.h"


#define LOCTEXT_NAMESPACE "CardEffects"


void UCardEffectFragment_Execution_GameplayAbility::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
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

FText UCardEffectFragment_Execution_GameplayAbility::GetName_Implementation()
{
	if (!ensure(AbilityToExecute.Ability))
	{
		return FText::GetEmpty();
	}

	if (UGameplayAbility_CardExecution* AsGA_CE = Cast<UGameplayAbility_CardExecution>(AbilityToExecute.Ability->GetDefaultObject()))
	{
		return AsGA_CE->GetDescription();
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE