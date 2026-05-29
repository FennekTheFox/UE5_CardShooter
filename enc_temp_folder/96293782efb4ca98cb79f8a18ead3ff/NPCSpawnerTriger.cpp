#include "NPCSpawnerTrigger.h"
#include "NPCSpawner.h"
#include "Components/BoxComponent.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"

void UNPCSpawnerTrigger::Initialize(ANPCSpawner* InSpawner)
{
	Spawner = InSpawner;
}

void UNPCSpawnerTrigger::Shutdown()
{
	Spawner = nullptr;
}

void UNPCSpawnerBeginPlayTrigger::Initialize(ANPCSpawner* InSpawner)
{
	Super::Initialize(InSpawner);

	if (Spawner)
	{
		Spawner->RequestSpawn();
	}
}

void UNPCSpawnerEventTrigger::Initialize(ANPCSpawner* InSpawner)
{
	if (Spawner)
	{
		UAbilityAsync_WaitGameplayEvent* Task = UAbilityAsync_WaitGameplayEvent::WaitGameplayEventToActor(
			Spawner,
			EventTag,
			bMatchExactOnly
		);

		Task->EventReceived.AddDynamic(this, &UNPCSpawnerEventTrigger::Callback);
	}
}

void UNPCSpawnerEventTrigger::Callback(FGameplayEventData Data)
{
	if (Spawner)
	{
		Spawner->RequestSpawn();
	}
}

void UNPCSpawnerBoxOverlapTrigger::Initialize(ANPCSpawner* InSpawner)
{
	Super::Initialize(InSpawner);

	if (!Spawner)
	{
		return;
	}

	BoxComponent = NewObject<UBoxComponent>(Spawner);
	BoxComponent->RegisterComponent();
	BoxComponent->AttachToComponent(
		Spawner->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);

	BoxComponent->SetRelativeLocation(RelativeLocation);
	BoxComponent->SetBoxExtent(BoxExtent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&UNPCSpawnerBoxOverlapTrigger::HandleBeginOverlap);
}

void UNPCSpawnerBoxOverlapTrigger::Shutdown()
{
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.RemoveAll(this);
		BoxComponent->DestroyComponent();
		BoxComponent = nullptr;
	}

	Super::Shutdown();
}

void UNPCSpawnerBoxOverlapTrigger::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!Spawner || !OtherActor)
	{
		return;
	}

	if (bTriggerOnce && bHasTriggered)
	{
		return;
	}

	if (RequiredActorClass && !OtherActor->IsA(RequiredActorClass))
	{
		return;
	}

	bHasTriggered = true;
	Spawner->RequestSpawn();
}