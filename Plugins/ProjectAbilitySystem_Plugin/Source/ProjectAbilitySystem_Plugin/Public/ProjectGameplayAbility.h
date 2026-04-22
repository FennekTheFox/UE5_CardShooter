#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"
#include "AbilityTriggers.h"
#include "ProjectGameplayAbility.generated.h"

class UInputAction;

//The project specific extension of the Gameplay Ability
UCLASS(BlueprintType, Blueprintable)
class PROJECTABILITYSYSTEM_PLUGIN_API UProjectGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public: 
	// Defines when this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Ability Activation")
	TArray<TObjectPtr<UAbilityActivationTrigger>> ActivationTriggers;

	//The input action this ability is associated with
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction;

public:
	void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
};