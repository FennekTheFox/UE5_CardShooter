#include "CardAttributeFragments_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../Actors/CardShooterCharacter.h"

void UCardEffectFragment_Execution::Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData AdditionalData /*= FCardExecution_AdditionalData()*/)
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

void UCardEffectFragment_Execution::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{

}

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

void UCardEffectFragment_Execution_DebugPrint::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	GEngine->AddOnScreenDebugMessage(-1, DisplayDuration, DisplayColor, Message);
}

void UCardEffectFragment_Execution_ApplyEffectToOwner::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	if (!ensure(ExecutingPawn && GameplayEffect))
	{
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ExecutingPawn);

	if (!ensure(ASC))
	{
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, GameplayEffectLevel, ASC->MakeEffectContext());

	FActiveGameplayEffectHandle EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	//TWeakObjectPtr<UCardEffectFragment_Execution_ApplyEffectToOwner> WeakThis = this;

	//ASC->AddLooseGameplayTag(TrackingTag);
	//ASC->OnGameplayEffectRemoved_InfoDelegate(EffectHandle)->AddLambda([ASC, WeakThis](const FGameplayEffectRemovalInfo&) {
	//	if(WeakThis.IsValid())
	//	{
	//		ASC->RemoveLooseGameplayTag(WeakThis->TrackingTag);
	//	};
}

void UCardEffectFragment_Execution_Compound::FindSubExecutionsOfClass(TSubclassOf<UCardEffectFragment_Execution> Class, TArray<UCardEffectFragment_Execution*>& OutArray)
{
	for (UCardEffectFragment_Execution* SubExec : Executions)
	{
		if (UCardEffectFragment_Execution_Compound* AsCompound = Cast<UCardEffectFragment_Execution_Compound>(SubExec))
		{
			AsCompound->FindSubExecutionsOfClass(Class, OutArray);
		}
		else if (SubExec->IsA(Class))
		{
			OutArray.Add(SubExec);
		}
	}
}

void UCardEffectFragment_Execution_AddAttributeToNextCards::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	//TODO This may not be player exclusive, but probably will. If not this needs to change
	if (auto* AsCSPCharacter = Cast<ACardShooterCharacter>(ExecutingPawn))
	{
		int32& ExistingCount = AsCSPCharacter->AttributesToAddToExecutions.FindOrAdd(Attribute);
		ExistingCount += Count;
	}
}

void UCardEffectFragment_Execution_RemoveAttributeFromNextCards::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData)
{
	//TODO This may not be player exclusive, but probably will. If not this needs to change
	if (auto* AsCSPCharacter = Cast<ACardShooterCharacter>(ExecutingPawn))
	{
		int32& ExistingCount = AsCSPCharacter->AttributesToRemoveFromExecutions.FindOrAdd(AttributeClass);
		ExistingCount += Count;
	}
}

void UCardEffectFragment_Execution_AllAtOnce::ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData /*= FCardExecution_AdditionalData()*/)
{
	for (UCardEffectFragment_Execution* Exec : Executions)
	{
		Exec->ExecuteCardEffect_Implementation(ExecutingPawn, AdditionalData);
	}
}