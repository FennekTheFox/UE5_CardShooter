#pragma once

#include "CardAttributeFragment_Execution.h"
#include "NativeGameplayTags.h"

#include "CardEffectFragment_GameplayAbility.generated.h"

UCLASS()
class UCardExecutionInstance_GameplayAbility : public UCardExecutionInstance
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_GameplayAbility 
	: public UCardEffectFragment_Execution_WithParameters
{
	GENERATED_BODY()

	UCardEffectFragment_GameplayAbility();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayAbilitySpecDef AbilityToExecute;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FString, TSubclassOf<class UGameplayEffect_CardExecution>> ReferencedEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag TargetDataID;
	//~UCardEffectFragment
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) override;
	 //~UCardEffectFragment

	void GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity) override;

};