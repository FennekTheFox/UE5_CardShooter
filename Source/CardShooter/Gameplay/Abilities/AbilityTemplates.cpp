#include "AbilityTemplates.h"

#define TC_ONGRANTED UAbilityActivationTrigger_OnAbilityGranted

UCardExecutionAbility_Base::UCardExecutionAbility_Base()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;

	ActivationTriggers.Add(Cast<TC_ONGRANTED>(TC_ONGRANTED::StaticClass()->GetDefaultObject(false)));
}

