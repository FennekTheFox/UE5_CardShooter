#pragma once

#include "CardAttributeFragment_Base.h"
#include "GameplayAbilitySpec.h"

#include "CardAttributeFragments_Execution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Execution : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:
	//Execution of cards always needs to happen on server side, and be replicated to the client
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ExecuteCardEffect(APawn* ExecutingPawn);
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Activate Gameplay Ability"))
class UCardEffectFragment_Execution_GameplayAbility : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAbilitySpecDef AbilityToExecute;

	void Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn) override;

};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Debug Print"))
class UCardEffectFragment_Execution_DebugPrint : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString Message;

	UPROPERTY(EditDefaultsOnly)
	FColor DisplayColor = FColor::Green;

	UPROPERTY(EditDefaultsOnly)
	float DisplayDuration = 2.f;

	void Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn) override;
};