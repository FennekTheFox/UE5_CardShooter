#include "GameplayAbility_CardExecution.h"

#include "Gameplay/GameplayEffect_CardExecution.h"
#include "AbilityTriggers.h"

#define TC_ONGRANTED UAbilityActivationTrigger_OnAbilityGranted

#define LOCTEXT_NAMESPACE "CardEffects"

UGameplayAbility_CardExecution::UGameplayAbility_CardExecution()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	ActivationTriggers.Add(Cast<TC_ONGRANTED>(TC_ONGRANTED::StaticClass()->GetDefaultObject(false)));
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