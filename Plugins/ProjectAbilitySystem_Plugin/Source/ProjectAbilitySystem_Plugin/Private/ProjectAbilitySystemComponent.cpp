#include "ProjectAbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "AbilitySet.h"
#include "ProjectGameplayAbility.h"
#include "InputAction.h"

void UProjectAbilitySystemComponent::AddAbilitySet(const FAbilitySet& AbilitySet, FGameplayTag ContextKey)
{
	// To ensure proper management the same context key cannot be used twice
	if (ContextData.Contains(ContextKey))
	{
		return;
	}

	FAbilitySetApplicationData& NewData = ContextData.FindOrAdd(ContextKey);

	// Give the abilities
	for (const FGameplayAbilitySpecDef& AbilitySpecDef : AbilitySet.Abilities)
	{
		if (!AbilitySpecDef.Ability)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySpecDef.Ability);
		NewData.GivenAbilities.Add(GiveAbility(AbilitySpec));

		auto* CDO = AbilitySpecDef.Ability->GetDefaultObject<UGameplayAbility>();
	}

	// Give the attribute sets
	for (TSubclassOf<UAttributeSet> AttributeSetClass : AbilitySet.Attributes)
	{
		UAttributeSet* NewSet = NewObject<UAttributeSet>(GetTransientPackage(), AttributeSetClass);
		NewData.SpawnedAttributes.Add(NewSet);
		AddSpawnedAttribute(NewSet);
	}

	// Apply the effects
	for (const FGameplayEffectSpec Spec : AbilitySet.Effects)
	{
		NewData.AppliedEffects.Add(ApplyGameplayEffectSpecToSelf(Spec));
	}
}

void UProjectAbilitySystemComponent::RemoveAbilitySet(const FAbilitySet& AbilitySet, FGameplayTag ContextKey)
{
	// Ensure the context key has been added to begin with
	if (!ContextData.Contains(ContextKey))
	{
		return;
	}

	FAbilitySetApplicationData* Data = ContextData.Find(ContextKey);

	if (!ensure(Data))
	{
		return;
	}

	// Clear the abilities
	for (FGameplayAbilitySpecHandle& AbilityHandle : Data->GivenAbilities)
	{
		ClearAbility(AbilityHandle);
	}

	// Remove the attribute sets
	for (TSubclassOf<UAttributeSet> AttributeSetClass : AbilitySet.Attributes)
	{
		const TArray<UAttributeSet*>& AttributeSets = GetSpawnedAttributes();

		for (UAttributeSet* AttributeSet : AttributeSets)
		{
			if (Data->SpawnedAttributes.Contains(AttributeSet))
			{
				RemoveSpawnedAttribute(AttributeSet);
			}
		}
	}

	// Remove the effects
	for (FActiveGameplayEffectHandle& EffectHandle : Data->AppliedEffects)
	{
		RemoveActiveGameplayEffect(EffectHandle);
	}

	ContextData.Remove(ContextKey);
}

void UProjectAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	if (APawn* Pawn = Cast<APawn>(GetAvatarActor()))
	{
		if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
		{
			BindAbilityInputs(AbilitySpec, PC);
		}
	}

	UProjectGameplayAbility* Ability = Cast<UProjectGameplayAbility>(AbilitySpec.Ability);

	for (UAbilityActivationTrigger* Trigger : Ability->ActivationTriggers)
	{
		if (Trigger->GetTriggerName() == FName("OnTagAdded"))
		{
			UAbilityActivationTrigger_OnTagAdded* AsOnTagAdded = Cast<UAbilityActivationTrigger_OnTagAdded>(Trigger);
			if (!ensure(AsOnTagAdded))
			{
				break;
			}
			auto& OnTagAddedAbilityList = AbilitiesToTriggerOnTagAdded.FindOrAdd(AsOnTagAdded->GameplayTag);

			if (OnTagAddedAbilityList.IsEmpty())
			{
				auto DelegateHandle = RegisterGameplayTagEvent(AsOnTagAdded->GameplayTag, EGameplayTagEventType::NewOrRemoved)
										  .AddUObject(this, &UProjectAbilitySystemComponent::OnGameplayTagNewOrRemoved_HandleOnTagAdded);
				OnTagAddedDelegateHandles.Add(AsOnTagAdded->GameplayTag, DelegateHandle);
			}

			OnTagAddedAbilityList.AddUnique(AbilitySpec.Handle);
		}
		else if (Trigger->GetTriggerName() == FName("OnGameplayEvent"))
		{
			UAbilityActivationTrigger_OnGameplayEvent* AsOnEventTrigger = Cast<UAbilityActivationTrigger_OnGameplayEvent>(Trigger);
			if (!ensure(AsOnEventTrigger))
			{
				break;
			}

			auto& EventAbilityList = AbilitiesToTriggerOnEvents.FindOrAdd(AsOnEventTrigger->GameplayTag);
			EventAbilityList.AddUnique(AbilitySpec.Handle);
		}
		else if (Trigger->GetTriggerName() == FName("WhileTagPresent"))
		{
			UAbilityActivationTrigger_WhileTagPresent* AsWhileTagPresent = Cast<UAbilityActivationTrigger_WhileTagPresent>(Trigger);
			if (!ensure(AsWhileTagPresent))
			{
				break;
			}
			auto& WhileTagPresentAbilityList = AbilitiesToTriggerWhileTagPresent.FindOrAdd(AsWhileTagPresent->GameplayTag);

			if (WhileTagPresentAbilityList.IsEmpty())
			{
				auto DelegateHandle = RegisterGameplayTagEvent(AsWhileTagPresent->GameplayTag, EGameplayTagEventType::NewOrRemoved)
										  .AddUObject(this, &UProjectAbilitySystemComponent::OnGameplayTagNewOrRemoved_WhileTagPresent);
				WhileTagPresentDelegateHandles.Add(AsWhileTagPresent->GameplayTag, DelegateHandle);
			}

			WhileTagPresentAbilityList.AddUnique(AbilitySpec.Handle);
		}
		else if (Trigger->GetTriggerName() == FName("WhileTagPresentOngoing"))
		{
			UAbilityActivationTrigger_WhileTagPresentOngoing* AsWhileTagPresentOngoing =
				Cast<UAbilityActivationTrigger_WhileTagPresentOngoing>(Trigger);
			if (!ensure(AsWhileTagPresentOngoing))
			{
				break;
			}
			auto& WhileTagPresentOngoingAbilityList =
				AbilitiesToTriggerWhileTagPresentOngoing.FindOrAdd(AsWhileTagPresentOngoing->GameplayTag);

			if (WhileTagPresentOngoingAbilityList.IsEmpty())
			{
				auto DelegateHandle =
					RegisterGameplayTagEvent(AsWhileTagPresentOngoing->GameplayTag, EGameplayTagEventType::NewOrRemoved)
						.AddUObject(this, &UProjectAbilitySystemComponent::OnGameplayTagNewOrRemoved_WhileTagPresentOngoing);
				WhileTagPresentOngoingDelegateHandles.Add(AsWhileTagPresentOngoing->GameplayTag, DelegateHandle);
			}

			WhileTagPresentOngoingAbilityList.AddUnique(AbilitySpec.Handle);
		}
	}
}

void UProjectAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);

	if (APawn* Pawn = Cast<APawn>(GetAvatarActor()))
	{
		if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
		{
			UnbindAbilityInputs(AbilitySpec, PC);
		}
	}

	UProjectGameplayAbility* Ability = Cast<UProjectGameplayAbility>(AbilitySpec.Ability);

	for (UAbilityActivationTrigger* Trigger : Ability->ActivationTriggers)
	{
		if (Trigger->GetTriggerName() == FName("OnTagAdded"))
		{
			UAbilityActivationTrigger_OnTagAdded* AsOnTagAdded = Cast<UAbilityActivationTrigger_OnTagAdded>(Trigger);
			if (!ensure(AsOnTagAdded))
			{
				break;
			}

			auto* OnTagAddedAbilityList = AbilitiesToTriggerOnTagAdded.Find(AsOnTagAdded->GameplayTag);
			if (OnTagAddedAbilityList)
			{
				OnTagAddedAbilityList->Remove(AbilitySpec.Handle);
				if (OnTagAddedAbilityList->Num() == 0)
				{
					AbilitiesToTriggerOnTagAdded.Remove(AsOnTagAdded->GameplayTag);
					FDelegateHandle* DelegateHandle = OnTagAddedDelegateHandles.Find(AsOnTagAdded->GameplayTag);
					if (ensure(DelegateHandle))
					{
						UnregisterGameplayTagEvent(*DelegateHandle, AsOnTagAdded->GameplayTag, EGameplayTagEventType::NewOrRemoved);
					}
				}
			}
		}
		else if (Trigger->GetTriggerName() == FName("OnGameplayEvent"))
		{
			UAbilityActivationTrigger_OnGameplayEvent* AsOnEventTrigger = Cast<UAbilityActivationTrigger_OnGameplayEvent>(Trigger);
			if (!ensure(AsOnEventTrigger))
			{
				break;
			}

			auto* EventAbilityList = AbilitiesToTriggerOnEvents.Find(AsOnEventTrigger->GameplayTag);
			if (EventAbilityList)
			{
				EventAbilityList->Remove(AbilitySpec.Handle);
				if (EventAbilityList->Num() == 0)
				{
					AbilitiesToTriggerOnEvents.Remove(AsOnEventTrigger->GameplayTag);
				}
			}
		}
		else if (Trigger->GetTriggerName() == FName("WhileTagPresent"))
		{
			UAbilityActivationTrigger_WhileTagPresent* AsWhileTagPresent = Cast<UAbilityActivationTrigger_WhileTagPresent>(Trigger);
			if (!ensure(AsWhileTagPresent))
			{
				break;
			}

			auto* WhileTagPresentAbilityList = AbilitiesToTriggerWhileTagPresent.Find(AsWhileTagPresent->GameplayTag);
			if (WhileTagPresentAbilityList)
			{
				WhileTagPresentAbilityList->Remove(AbilitySpec.Handle);
				if (WhileTagPresentAbilityList->Num() == 0)
				{
					AbilitiesToTriggerWhileTagPresent.Remove(AsWhileTagPresent->GameplayTag);
					FDelegateHandle* DelegateHandle = WhileTagPresentDelegateHandles.Find(AsWhileTagPresent->GameplayTag);
					if (ensure(DelegateHandle))
					{
						UnregisterGameplayTagEvent(*DelegateHandle, AsWhileTagPresent->GameplayTag, EGameplayTagEventType::NewOrRemoved);
					}
				}
			}
		}
		else if (Trigger->GetTriggerName() == FName("WhileTagPresentOngoing"))
		{
			UAbilityActivationTrigger_WhileTagPresentOngoing* AsWhileTagPresentOngoing =
				Cast<UAbilityActivationTrigger_WhileTagPresentOngoing>(Trigger);
			if (!ensure(AsWhileTagPresentOngoing))
			{
				break;
			}

			auto* WhileTagPresentOngoingAbilityList = AbilitiesToTriggerWhileTagPresentOngoing.Find(AsWhileTagPresentOngoing->GameplayTag);
			if (WhileTagPresentOngoingAbilityList)
			{
				WhileTagPresentOngoingAbilityList->Remove(AbilitySpec.Handle);
				if (WhileTagPresentOngoingAbilityList->Num() == 0)
				{
					AbilitiesToTriggerWhileTagPresentOngoing.Remove(AsWhileTagPresentOngoing->GameplayTag);
					FDelegateHandle* DelegateHandle = WhileTagPresentOngoingDelegateHandles.Find(AsWhileTagPresentOngoing->GameplayTag);
					if (ensure(DelegateHandle))
					{
						UnregisterGameplayTagEvent(*DelegateHandle, AsWhileTagPresentOngoing->GameplayTag,
												   EGameplayTagEventType::NewOrRemoved);
					}
				}
			}
			AbilitiesToTriggerOnTick.Remove(AbilitySpec.Handle);
		}
	}
}

void UProjectAbilitySystemComponent::BindAbilityInputs(FGameplayAbilitySpec& Spec, APlayerController* PC)
{
	UProjectGameplayAbility* Ability = Cast<UProjectGameplayAbility>(Spec.Ability);

	if (!Ability)
	{
		return;
	}

	if (Ability->InputAction)
	{
		if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			FInputActionAbilityBindings* AbilityBindings = AbilityBindingsPerInputAction.Find(Ability->InputAction);
			if (!AbilityBindings)
			{
				AbilityBindings = &(AbilityBindingsPerInputAction.Add(Ability->InputAction));
				AbilityBindings->BindingTriggered = EnhancedInputComp
														->BindAction(Ability->InputAction.Get(), ETriggerEvent::Triggered, this,
																	 &ThisClass::AbilityInputActionTriggered, Ability->InputAction.Get())
														.GetHandle();
				AbilityBindings->BindingCompleted =
					EnhancedInputComp
						->BindAction(Ability->InputAction.Get(), ETriggerEvent::Completed, this,
									 &ThisClass::AbilityInputActionCancelledOrCompleted, Ability->InputAction.Get())
						.GetHandle();
				AbilityBindings->BindingCancelled =
					EnhancedInputComp
						->BindAction(Ability->InputAction.Get(), ETriggerEvent::Canceled, this,
									 &ThisClass::AbilityInputActionCancelledOrCompleted, Ability->InputAction.Get())
						.GetHandle();
			}
			AbilityBindings->Abilities.Add(Spec.Handle);
		}
	}
}

void UProjectAbilitySystemComponent::UnbindAbilityInputs(FGameplayAbilitySpec& Spec, APlayerController* PC)
{
	UProjectGameplayAbility* Ability = Cast<UProjectGameplayAbility>(Spec.Ability);

	if (!Ability)
	{
		return;
	}

	if (Ability->InputAction)
	{
		if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			FInputActionAbilityBindings* AbilityBindings = AbilityBindingsPerInputAction.Find(Ability->InputAction);
			if (AbilityBindings)
			{
				AbilityBindings->Abilities.Remove(Spec.Handle);
				if (AbilityBindings->Abilities.Num() == 0)
				{
					EnhancedInputComp->RemoveBindingByHandle(AbilityBindings->BindingTriggered);
					EnhancedInputComp->RemoveBindingByHandle(AbilityBindings->BindingCompleted);
					EnhancedInputComp->RemoveBindingByHandle(AbilityBindings->BindingCancelled);
					AbilityBindingsPerInputAction.Remove(Ability->InputAction);
				}
			}
		}
	}
}

void UProjectAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// Process all abilities with buffered activation inputs
	for (auto It = BufferedAbilityActivations.CreateIterator(); It; ++It)
	{
		if (It->Value > GetWorld()->GetRealTimeSeconds())
		{
			AbilitiesToActivate.AddUnique(It->Key);
		}
		else
		{
			It.RemoveCurrent();
		}
	}

	// Process all abilities that had their input action triggered this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputTriggeredSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true; // InputPressed to us means "input was triggered and not completed/cancelled since"

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);

					if (AbilitySpec->IsActive())
					{
						// Replicated Event if active
						TArray<UGameplayAbility*> Instances = AbilitySpec->GetAbilityInstances();
						const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
						InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, SpecHandle,
											  ActivationInfo.GetActivationPredictionKey());
					}
				}
				else
				{
					if (const UProjectGameplayAbility* AbilityCDO = CastChecked<UProjectGameplayAbility>(AbilitySpec->Ability))
					{
						for (UAbilityActivationTrigger* ActivationTrigger : AbilityCDO->ActivationTriggers)
						{
							if (UAbilityActivationTrigger_OnInputTriggered* OnInputTriggered =
									Cast<UAbilityActivationTrigger_OnInputTriggered>(ActivationTrigger))
							{
								AbilitiesToActivate.AddUnique(AbilitySpec->Handle);

								if (OnInputTriggered->InputBufferDuration > 0.f)
								{
									BufferedAbilityActivations.Add(SpecHandle, GetWorld()->GetRealTimeSeconds() +
																				   OnInputTriggered->InputBufferDuration);
								}

								break;
							}
						}
					}
				}
			}
		}
	}

	// Try to activate all the abilities that are from (buffered) input triggers.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		const bool bBegunToActivateSuccessfully = TryActivateAbility(AbilitySpecHandle);
		if (bBegunToActivateSuccessfully)
		{
			BufferedAbilityActivations.Remove(AbilitySpecHandle);
		}
	}

	// Process all abilities that had their input action completed this frame.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputCompletedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false; // InputPressed to us means "input was triggered and not completed/cancelled since"

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);

					if (AbilitySpec->IsActive())
					{
						// Replicated Event if active
						TArray<UGameplayAbility*> Instances = AbilitySpec->GetAbilityInstances();
						const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
						InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, SpecHandle,
											  ActivationInfo.GetActivationPredictionKey());
					}
				}
			}
		}
	}

	// Clear the cached ability handles.
	ClearAbilityInput();
}

void UProjectAbilitySystemComponent::AbilityInputActionTriggered(const FInputActionInstance& InputActionInstance,
																	UInputAction* InputAction)
{
	if (InputAction)
	{
		FInputActionAbilityBindings* AbilityBindings = AbilityBindingsPerInputAction.Find(InputAction);
		if (AbilityBindings)
		{
			for (FGameplayAbilitySpecHandle Handle : AbilityBindings->Abilities)
			{
				InputTriggeredSpecHandles.AddUnique(Handle);
			}
		}
	}
}

void UProjectAbilitySystemComponent::AbilityInputActionCancelledOrCompleted(const FInputActionInstance& InputActionInstance,
																			   UInputAction* InputAction)
{
	if (InputAction)
	{
		FInputActionAbilityBindings* AbilityBindings = AbilityBindingsPerInputAction.Find(InputAction);
		if (AbilityBindings)
		{
			for (FGameplayAbilitySpecHandle Handle : AbilityBindings->Abilities)
			{
				InputCompletedSpecHandles.AddUnique(Handle);
			}
		}
	}
}

void UProjectAbilitySystemComponent::ClearAbilityInput()
{
	InputTriggeredSpecHandles.Reset();
	InputCompletedSpecHandles.Reset();
}

void UProjectAbilitySystemComponent::OnGameplayTagNewOrRemoved_HandleOnTagAdded(FGameplayTag Tag, int32 count)
{
	if (count > 0)
	{
		TArray<FGameplayAbilitySpecHandle>* HandlesToTrigger = AbilitiesToTriggerOnTagAdded.Find(Tag);
		if (HandlesToTrigger)
		{
			for (FGameplayAbilitySpecHandle Handle : *HandlesToTrigger)
			{
				TryActivateAbility(Handle);
			}
		}
	}
}

void UProjectAbilitySystemComponent::OnGameplayTagNewOrRemoved_WhileTagPresent(FGameplayTag Tag, int32 count)
{
	TArray<FGameplayAbilitySpecHandle>* HandlesToTrigger = AbilitiesToTriggerOnTagAdded.Find(Tag);
	if (HandlesToTrigger)
	{
		for (FGameplayAbilitySpecHandle Handle : *HandlesToTrigger)
		{
			if (count > 0)
			{
				TryActivateAbility(Handle);
			}
			else
			{
				CancelAbilityHandle(Handle);
			}
		}
	}
}

void UProjectAbilitySystemComponent::OnGameplayTagNewOrRemoved_WhileTagPresentOngoing(FGameplayTag Tag, int32 count)
{
	TArray<FGameplayAbilitySpecHandle>* HandlesToTrigger = AbilitiesToTriggerOnTagAdded.Find(Tag);
	if (HandlesToTrigger)
	{
		for (FGameplayAbilitySpecHandle Handle : *HandlesToTrigger)
		{
			if (count > 0)
			{
				TryActivateAbility(Handle);
				AbilitiesToTriggerOnTick.AddUnique(Handle);
			}
			else
			{
				CancelAbilityHandle(Handle);
				AbilitiesToTriggerOnTick.Remove(Handle);
			}
		}
	}
}

void UProjectAbilitySystemComponent::OnPawnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (APlayerController* OldPlayerController = Cast<APlayerController>(OldController))
	{
		for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			UnbindAbilityInputs(Spec, OldPlayerController);
		}
	}

	if (APlayerController* NewPlayerController = Cast<APlayerController>(NewController))
	{
		for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			BindAbilityInputs(Spec, NewPlayerController);
		}
	}
}

void UProjectAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UProjectGameplayAbility* AbilityCDO = CastChecked<UProjectGameplayAbility>(AbilitySpec.Ability))
		{
			AbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}

void UProjectAbilitySystemComponent::AddDefaults(AActor* InOwnerActor, AActor* InAvatarActor)
{
	for (auto&& KVPair : DefaultAbilitySets)
	{
		if (!ensure(KVPair.Value))
		{
			continue;
		}

		AddAbilitySet(KVPair.Value->AbilitySet, KVPair.Key);
	}
}

void UProjectAbilitySystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
													  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FGameplayAbilitySpecHandle Handle : AbilitiesToTriggerOnTick)
	{
		TryActivateAbility(Handle);
	}
}

int32 UProjectAbilitySystemComponent::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (auto* AbilitiesToTrigger = AbilitiesToTriggerOnEvents.Find(EventTag))
	{
		for (FGameplayAbilitySpecHandle Handle : *AbilitiesToTrigger)
		{
			TryActivateAbility(Handle);
		}
	}

	return Super::HandleGameplayEvent(EventTag, Payload);
}

void UProjectAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	if (!ensure(ActorInfo) || !ensure(InOwnerActor))
	{
		return;
	}

	const bool bAvatarChanged = (InAvatarActor != AbilityActorInfo->AvatarActor);

	if (bAvatarChanged)
	{
		// remove callbacks from previous avatar
		if (APawn* Pawn = Cast<APawn>(AbilityActorInfo->AvatarActor))
		{
			Pawn->ReceiveControllerChangedDelegate.RemoveDynamic(this, &UProjectAbilitySystemComponent::OnPawnControllerChanged);
		}
	}

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bAvatarChanged)
	{
		AddDefaults(InOwnerActor, InAvatarActor);

		// add callbacks to new avatar
		if (APawn* Pawn = Cast<APawn>(GetAvatarActor()))
		{
			Pawn->ReceiveControllerChangedDelegate.AddDynamic(this, &UProjectAbilitySystemComponent::OnPawnControllerChanged);
		}

		TryActivateAbilitiesOnSpawn();
	}
}
