#include "CardEffectFragment_GameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GameplayAbility_CardExecution.h"


#define LOCTEXT_NAMESPACE "CardEffects"

namespace CardGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Event_InitializeCardAbility, "Gameplay.Event.InitializeCardAbility");
}

void UCardExecutionInstance_GameplayAbility::ProcessExecutionInstance()
{
	UCardEffectFragment_GameplayAbility* AsGA = Cast<UCardEffectFragment_GameplayAbility>(Exec);

	if (!ensure(AsGA))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);
		return;
	}

	FGameplayAbilitySpec AbilitySpec(AsGA->AbilityToExecute.Ability, 1, -1, this);	
	FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(AbilitySpec);

	//This relies on the ability 100% getting executed, else it wont get removed properly
	TSharedPtr<FDelegateHandle> OnEndedHandle = MakeShared<FDelegateHandle>();
	TSharedPtr<FDelegateHandle> OnFailedHandle = MakeShared<FDelegateHandle>();
	*OnEndedHandle = ASC->OnAbilityEnded.AddLambda([this, ASC, SpecHandle, AbilitySpec, OnEndedHandle, OnFailedHandle](const FAbilityEndedData& Data) {
		if (Data.AbilitySpecHandle == SpecHandle)
		{
			ASC->ClearAbility(SpecHandle);
			OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);

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

	*OnFailedHandle = ASC->AbilityFailedCallbacks.AddLambda([this, ASC, SpecHandle, AbilitySpec, OnEndedHandle, OnFailedHandle](const UGameplayAbility* Ability, const FGameplayTagContainer&) {
		if (Ability == AbilitySpec.Ability)
		{
			ASC->ClearAbility(SpecHandle);
			OnExecutionCompleted.Broadcast(ExecutingPawn, AdditionalData, Exec);

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

	ExecutingPawn->GetWorld()->GetTimerManager()
		.SetTimerForNextTick(FTimerDelegate::CreateLambda(
			[this, ASC, SpecHandle]
			()
			{

				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(SpecHandle);
				if (Spec)
				{
					//TODO: this triggers on certain abilities, which execute on server and client
					//not fatal, only annoying and not intended
					if (!/*ensure*/(Spec->GetAbilityInstances().IsEmpty()))
					{
						UGameplayAbility_CardExecution* GA_CE = Cast<UGameplayAbility_CardExecution>(Spec->GetAbilityInstances().Last());
						if (GA_CE)
						{
							GA_CE->AdditionalData = AdditionalData;
							GA_CE->PostAdditionalDataSet();
						}
					}
				}
			}));
}

UCardExecutionInstance* UCardEffectFragment_GameplayAbility::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	UCardExecutionInstance* NewInstance = NewObject<UCardExecutionInstance_GameplayAbility>(this);
	FCardExecution_AdditionalData AppendedData = AdditionalData;
	for (auto&& KVPair : SetByCallerMagnitudes)
	{
		AppendedData.SetByCallerMetaDataTags.Add(KVPair.Key);
		AppendedData.SetByCallerMetaDataValues.Add(KVPair.Value);
	}
	NewInstance->StartExecutionInstance(ExecutingPawn, AppendedData, this);
	return NewInstance;
}

FText UCardEffectFragment_GameplayAbility::GetLocalizableDescription_Implementation()
{
	if (!ensure(AbilityToExecute.Ability))
	{
		return FText::GetEmpty();
	}

	if (UGameplayAbility_CardExecution* AsGA_CE = Cast<UGameplayAbility_CardExecution>(AbilityToExecute.Ability->GetDefaultObject()))
	{
		FFormatNamedArguments Args;
		GetSetByCallerFormattingArgs(Args);

		return FText::Format(FText::FromString("{0}: {1}"), 
			AsGA_CE->GetName(), 
			FText::Format(AsGA_CE->GetDescription(), Args));
	}

	ensure(false);
	return FText::GetEmpty();
}


#undef LOCTEXT_NAMESPACE
