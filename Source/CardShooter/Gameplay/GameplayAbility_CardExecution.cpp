#include "GameplayAbility_CardExecution.h"

#include "AbilityTriggers.h"

#define TC_ONGRANTED UAbilityActivationTrigger_OnAbilityGranted

UGameplayAbility_CardExecution::UGameplayAbility_CardExecution()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	ActivationTriggers.Add(Cast<TC_ONGRANTED>(TC_ONGRANTED::StaticClass()->GetDefaultObject(false)));
}