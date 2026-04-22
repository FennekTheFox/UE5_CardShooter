#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "GameplayAbilitySpecHandle.h"

#include "ProjectAbilitySystemComponent.generated.h"

class UAbilitySetData;
class UInputAction;

struct PROJECTABILITYSYSTEM_PLUGIN_API FInputActionAbilityBindings
{
	TArray<FGameplayAbilitySpecHandle> Abilities;
	uint32 BindingTriggered;
	uint32 BindingCancelled;
	uint32 BindingCompleted;
};


USTRUCT()
struct PROJECTABILITYSYSTEM_PLUGIN_API FAbilitySetApplicationData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GivenAbilities;

	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> SpawnedAttributes;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> AppliedEffects;
};

//The project specific extension of the ability system component
//Reason why im subclassing this is to be able to define some default initializations and custom
//Ability activation policies that link EnhancedInput comfortably
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class PROJECTABILITYSYSTEM_PLUGIN_API UProjectAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//BEGIN UActorComponent
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//END UActorComponent

	//The default ability sets to be initialized on the actor
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, TObjectPtr<UAbilitySetData>> DefaultAbilitySets;

	UFUNCTION(BlueprintCallable)
	void AddAbilitySet(UPARAM(ref) const FAbilitySet& AbilitySet, FGameplayTag ContextKey);
	UFUNCTION(BlueprintCallable)
	void RemoveAbilitySet(UPARAM(ref) const FAbilitySet& AbilitySet, FGameplayTag ContextKey);

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload) override;
	void BindAbilityInputs(FGameplayAbilitySpec& Spec, APlayerController* PC);
	void UnbindAbilityInputs(FGameplayAbilitySpec& Spec, APlayerController* PC);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void AbilityInputActionTriggered(const FInputActionInstance& InputActionInstance, UInputAction* InputAction);
	void AbilityInputActionCancelledOrCompleted(const FInputActionInstance& InputActionInstance, UInputAction* InputAction);
	void ClearAbilityInput();
	
	void OnGameplayTagNewOrRemoved_HandleOnTagAdded(FGameplayTag Tag, int32 count);
	void OnGameplayTagNewOrRemoved_WhileTagPresent(FGameplayTag Tag, int32 count);
	void OnGameplayTagNewOrRemoved_WhileTagPresentOngoing(FGameplayTag Tag, int32 count);

	UFUNCTION()
	void OnPawnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);
	void TryActivateAbilitiesOnSpawn();

	void AddDefaults(AActor* InOwnerActor, AActor* InAvatarActor);

private:
	UPROPERTY()
	TMap<FGameplayTag, FAbilitySetApplicationData> ContextData;

	// Handles to abilities that had their input action triggered this frame.
	TArray<FGameplayAbilitySpecHandle> InputTriggeredSpecHandles;

	// Handles to abilities that had their input action completed this frame.
	TArray<FGameplayAbilitySpecHandle> InputCompletedSpecHandles;

	TMap<FGameplayAbilitySpecHandle, float> BufferedAbilityActivations;

	TMap<TWeakObjectPtr<const UInputAction>, FInputActionAbilityBindings> AbilityBindingsPerInputAction;
	
	// Handles to abilities that need to be triggered on Gameplay Event
	TMap<FGameplayTag, TArray<FGameplayAbilitySpecHandle>> AbilitiesToTriggerOnEvents;

	// Handles to abilities that need to be triggered on TagAdded
	TMap<FGameplayTag, TArray<FGameplayAbilitySpecHandle>> AbilitiesToTriggerOnTagAdded;
	
	// Handles to abilities that need to be triggered on TagAdded
	TMap<FGameplayTag, TArray<FGameplayAbilitySpecHandle>> AbilitiesToTriggerWhileTagPresent;

	// Handles to abilities that need to be triggered on TagAdded
	TMap<FGameplayTag, TArray<FGameplayAbilitySpecHandle>> AbilitiesToTriggerWhileTagPresentOngoing;

	TArray<FGameplayAbilitySpecHandle> AbilitiesToTriggerOnTick;
	
	TMap<FGameplayTag, FDelegateHandle> OnTagAddedDelegateHandles;
	TMap<FGameplayTag, FDelegateHandle> WhileTagPresentDelegateHandles;
	TMap<FGameplayTag, FDelegateHandle> WhileTagPresentOngoingDelegateHandles;

public:
	void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
};
