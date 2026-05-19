#include "AttributeDrivenMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Gameplay/Attributes/MovementAttributeSet.h"

UAttributeDrivenCharacterMovementComponent::
UAttributeDrivenCharacterMovementComponent()
{
}

void UAttributeDrivenCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeFromAbilitySystem();
}

void UAttributeDrivenCharacterMovementComponent::EndPlay(
	const EEndPlayReason::Type EndPlayReason)
{
	UnbindAttributeDelegates();

	Super::EndPlay(EndPlayReason);
}

void UAttributeDrivenCharacterMovementComponent::InitializeFromAbilitySystem()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	AbilitySystemComponent =
		UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);

	if (!AbilitySystemComponent)
	{
		return;
	}

	BindAttributeDelegates();
	ApplyAllMovementAttributes();
}

void UAttributeDrivenCharacterMovementComponent::BindAttributeDelegates()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	UnbindAttributeDelegates();

	MoveSpeedHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetMoveSpeedAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleMoveSpeedChanged);

	RotationRateHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetRotationRateAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleRotationRateChanged);

	JumpHeightHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetJumpHeightAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleJumpHeightChanged);

	AccelerationHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetAccelerationAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleAccelerationChanged);

	BrakingDecelerationHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetBrakingDecelerationAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleBrakingDecelerationChanged);

	AirControlHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetAirControlAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleAirControlChanged);

	GravityScaleHandle =
		AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetGravityScaleAttribute())
		.AddUObject(
			this,
			&UAttributeDrivenCharacterMovementComponent::HandleGravityScaleChanged);
}

void UAttributeDrivenCharacterMovementComponent::UnbindAttributeDelegates()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetMoveSpeedAttribute())
		.Remove(MoveSpeedHandle);

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetRotationRateAttribute())
		.Remove(RotationRateHandle);

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetJumpHeightAttribute())
		.Remove(JumpHeightHandle);

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetAccelerationAttribute())
		.Remove(AccelerationHandle);

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetBrakingDecelerationAttribute())
		.Remove(BrakingDecelerationHandle);

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetAirControlAttribute())
		.Remove(AirControlHandle);

	AbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(
			UMovementAttributeSet::GetGravityScaleAttribute())
		.Remove(GravityScaleHandle);

	MoveSpeedHandle.Reset();
	RotationRateHandle.Reset();
	JumpHeightHandle.Reset();
	AccelerationHandle.Reset();
	BrakingDecelerationHandle.Reset();
	AirControlHandle.Reset();
	GravityScaleHandle.Reset();
}

void UAttributeDrivenCharacterMovementComponent::ApplyAllMovementAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	ApplyMoveSpeed(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetMoveSpeedAttribute()));

	ApplyRotationRate(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetRotationRateAttribute()));

	ApplyJumpHeight(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetJumpHeightAttribute()));

	ApplyAcceleration(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetAccelerationAttribute()));

	ApplyBrakingDeceleration(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetBrakingDecelerationAttribute()));

	ApplyAirControl(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetAirControlAttribute()));

	ApplyGravityScale(
		AbilitySystemComponent->GetNumericAttribute(
			UMovementAttributeSet::GetGravityScaleAttribute()));
}

void UAttributeDrivenCharacterMovementComponent::HandleMoveSpeedChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyMoveSpeed(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::HandleRotationRateChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyRotationRate(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::HandleJumpHeightChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyJumpHeight(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::HandleAccelerationChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyAcceleration(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::HandleBrakingDecelerationChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyBrakingDeceleration(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::HandleAirControlChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyAirControl(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::HandleGravityScaleChanged(
	const FOnAttributeChangeData& Data)
{
	ApplyGravityScale(Data.NewValue);
}

void UAttributeDrivenCharacterMovementComponent::ApplyMoveSpeed(float NewValue)
{
	MaxWalkSpeed = NewValue;
}

void UAttributeDrivenCharacterMovementComponent::ApplyRotationRate(float NewValue)
{
	RotationRate = FRotator(0.f, NewValue, 0.f);
}

void UAttributeDrivenCharacterMovementComponent::ApplyJumpHeight(float NewValue)
{
	JumpZVelocity = NewValue;
}

void UAttributeDrivenCharacterMovementComponent::ApplyAcceleration(float NewValue)
{
	MaxAcceleration = NewValue;
}

void UAttributeDrivenCharacterMovementComponent::ApplyBrakingDeceleration(float NewValue)
{
	BrakingDecelerationWalking = NewValue;
}

void UAttributeDrivenCharacterMovementComponent::ApplyAirControl(float NewValue)
{
	AirControl = NewValue;
}

void UAttributeDrivenCharacterMovementComponent::ApplyGravityScale(float NewValue)
{
	GravityScale = NewValue;
}