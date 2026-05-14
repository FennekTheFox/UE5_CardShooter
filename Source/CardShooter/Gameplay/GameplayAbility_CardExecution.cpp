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
	FGameplayEffectContextHandle Context = GetGrantedByEffectContext();

	UObject* SourceObject = Context.GetSourceObject();

	if (ensure(SourceObject))
	{
		//The source object could either be an instance, if the ability was granted by a GE which was applied via a card execution, or 
		//an ability execution if applied via an effect that was applied via an ability (yes i know its confusing)
		TMap<FGameplayTag, FSetByCallerMetaData>* SetByCallerData = nullptr;
		
		UCardEffectFragment_ExecutionWithParameters* WithParams = Cast<UCardEffectFragment_ExecutionWithParameters>(SourceObject);

		if (!WithParams)
		{
			UCardExecutionInstance* AsInstance = Cast<UCardExecutionInstance>(SourceObject);

			if (AsInstance)
			{
				WithParams = Cast<UCardEffectFragment_ExecutionWithParameters>(AsInstance->Exec);
			}
		}

		if (WithParams)
		{
			SetByCallerData = &WithParams->SetByCallerMagnitudes;
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