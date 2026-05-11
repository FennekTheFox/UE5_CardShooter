#include "GameplayTargeting_ClientCrosshair.h"

#include "Abilities/GameplayAbility.h"

AGameplayTageting_ClientCrosshair::AGameplayTageting_ClientCrosshair()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	ShouldProduceTargetDataOnServer = false;
}

void AGameplayTageting_ClientCrosshair::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability ? Ability->GetCurrentActorInfo()->AvatarActor.Get() : nullptr;
}

void AGameplayTageting_ClientCrosshair::ConfirmTargetingAndContinue()
{
	FHitResult HitResult = PerformCrosshairTrace();

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* HitData =
		new FGameplayAbilityTargetData_SingleTargetHit(HitResult);

	TargetDataHandle.Add(HitData);

	TargetDataReadyDelegate.Broadcast(TargetDataHandle);
}

bool AGameplayTageting_ClientCrosshair::IsConfirmTargetingAllowed()
{
	return true;
}

FHitResult AGameplayTageting_ClientCrosshair::PerformCrosshairTrace() const
{
	FHitResult HitResult;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
	{
		return HitResult;
	}

	FVector WorldOrigin;
	FVector WorldDirection;

	if (!PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		return HitResult;
	}

	const FVector TraceStart = WorldOrigin;
	const FVector TraceEnd = TraceStart + WorldDirection * TraceRange;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(PlayerCrosshairTargetActor), bTraceComplex);
	Params.bReturnPhysicalMaterial = false;

	if (SourceActor)
	{
		Params.AddIgnoredActor(SourceActor);
	}

	if (APawn* Pawn = PC->GetPawn())
	{
		Params.AddIgnoredActor(Pawn);
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return HitResult;
	}

	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		TraceChannel,
		Params
	);

	if (!bHit)
	{
		HitResult.TraceStart = TraceStart;
		HitResult.TraceEnd = TraceEnd;
		HitResult.Location = TraceEnd;
		HitResult.ImpactPoint = TraceEnd;
	}

	return HitResult;
}

APlayerController* AGameplayTageting_ClientCrosshair::GetOwningPlayerController() const
{
	if (OwningAbility)
	{
		const FGameplayAbilityActorInfo* ActorInfo = OwningAbility->GetCurrentActorInfo();
		if (ActorInfo && ActorInfo->PlayerController.IsValid())
		{
			return ActorInfo->PlayerController.Get();
		}
	}

	if (APawn* Pawn = Cast<APawn>(SourceActor))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	return nullptr;
}

