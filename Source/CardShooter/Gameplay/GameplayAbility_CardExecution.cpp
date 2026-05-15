#include "GameplayAbility_CardExecution.h"

#include "Gameplay/GameplayEffect_CardExecution.h"
#include "AbilityTriggers.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"

#define TC_ONGRANTED UAbilityActivationTrigger_OnAbilityGranted

#define LOCTEXT_NAMESPACE "CardEffects"

UGameplayAbility_CardExecution::UGameplayAbility_CardExecution()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	ActivationTriggers.Add(Cast<TC_ONGRANTED>(TC_ONGRANTED::StaticClass()->GetDefaultObject(false)));
}

bool UGameplayAbility_CardExecution::GetEffectContextSourceSetByCallerValue(FGameplayTag Tag, FSetByCallerMetaData& Out)
{
	//The source object could either be an instance, if the ability was granted by a GE which was applied via a card execution, or 
	//an ability execution if applied via an effect that was applied via an ability (yes i know its confusing). Either way we need
	//to retrieve the set by caller data from the _WithParameters fragment

	TMap<FGameplayTag, FSetByCallerMetaData>* SetByCallerData = nullptr;
	UCardEffectFragment_ExecutionWithParameters* WithParams = nullptr;

	FGameplayEffectContextHandle Context = GetGrantedByEffectContext();
	UObject* ContextSourceObject = Context.GetSourceObject();
	if ((ContextSourceObject))
	{
		WithParams = Cast<UCardEffectFragment_ExecutionWithParameters>(ContextSourceObject);

		if (!WithParams)
		{
			UCardExecutionInstance* AsInstance = Cast<UCardExecutionInstance>(ContextSourceObject);

			if (AsInstance)
			{
				WithParams = Cast<UCardEffectFragment_ExecutionWithParameters>(AsInstance->Exec);
			}
		}
	}

	//If added directly via the CEF_GA, it should be the Source Object
	if (!WithParams)
	{
		UCardExecutionInstance* AsInstance = Cast<UCardExecutionInstance>(GetCurrentSourceObject());

		if (AsInstance)
		{
			WithParams = Cast<UCardEffectFragment_ExecutionWithParameters>(AsInstance->Exec);
		}
	}		
	
	if (WithParams)
	{
		SetByCallerData = &(WithParams->SetByCallerMagnitudes);
	}

	if (ensure(SetByCallerData))
	{
		FSetByCallerMetaData* Data = WithParams->SetByCallerMagnitudes.Find(Tag);

		if (Data)
		{
			Out = *Data;
			return true;
		}
	}

	return false;
}

void UGameplayAbility_CardExecution::GetFormatingArguments(FFormatNamedArguments& Args)
{
	for (auto&& KVPair : ReferencedEffects)
	{
		if (!ensure(KVPair.Value))
		{
			continue;
		}

		Args.Add(KVPair.Key, FText::Format(LOCTEXT("AbilityReferencedEffectFormat",
			"<hover id=\"{0}\">{1}</>"),
			FText::FromString(KVPair.Value->GetDefaultObject()->GetPathName()),
			Cast<UGameplayEffect_CardExecution>(KVPair.Value->GetDefaultObject())->GetDetailedLocalizableName()
		));
	}
}

#undef LOCTEXT_NAMESPACE