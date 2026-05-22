#include "NPCSpawner.h"
#include "NPCSpawnerConfig.h"
#include "NPCSpawnerTrigger.h"

#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_IdleTaskInjection, "Gameplay.AI.Behaviour.InjectionTags.IdleTask");

ANPCSpawner::ANPCSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	DefaultSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("DefaultSpawnPoint"));
	DefaultSpawnPoint->SetupAttachment(Root);

	SpawnPoints.Add(DefaultSpawnPoint);
}

void ANPCSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return; 
	}

	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCSpawner %s has no config."), *GetName());
		return;
	}

	for (UNPCSpawnerTrigger* TriggerTemplate : Config->Triggers)
	{
		if (!TriggerTemplate)
		{
			continue;
		}

		UNPCSpawnerTrigger* RuntimeTrigger = DuplicateObject<UNPCSpawnerTrigger>(
			TriggerTemplate,
			this);

		if (!RuntimeTrigger)
		{
			continue;
		}

		RuntimeTriggers.Add(RuntimeTrigger);
		RuntimeTrigger->Initialize(this);
	}
}

void ANPCSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (UNPCSpawnerTrigger* Trigger : RuntimeTriggers)
	{
		if (Trigger)
		{
			Trigger->Shutdown();
		}
	}

	RuntimeTriggers.Empty();

	if (Config && Config->bDestroySpawnedOnSpawnerDestroyed)
	{
		for (APawn* Pawn : SpawnedPawns)
		{
			if (IsValid(Pawn))
			{
				Pawn->Destroy();
			}
		}
	}

	SpawnedPawns.Empty();

	Super::EndPlay(EndPlayReason);
}

bool ANPCSpawner::RequestSpawn()
{
	if (!HasAuthority())
	{
		return false;
	}

	if (!Config)
	{
		return false;
	}

	CleanupDeadPawns();

	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastSpawnTime < Config->SpawnCooldown)
	{
		return false;
	}

	if (GetAliveCount() >= Config->MaxAlive)
	{
		return false;
	}

	bool bSpawnedAny = false;

	for (int32 i = 0; i < Config->SpawnCountPerTrigger; ++i)
	{
		if (GetAliveCount() >= Config->MaxAlive)
		{
			break;
		}

		bSpawnedAny |= SpawnOne();
	}

	if (bSpawnedAny)
	{
		LastSpawnTime = Now;
	}

	return bSpawnedAny;
}

bool ANPCSpawner::SpawnOne()
{
	TSubclassOf<APawn> PawnClass = PickPawnClass();
	if (!PawnClass)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const FTransform SpawnTransform = PickSpawnTransform();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	APawn* SpawnedPawn = World->SpawnActor<APawn>(
		PawnClass,
		SpawnTransform,
		Params);

	if (!SpawnedPawn)
	{
		return false;
	}

	if (Config->bSpawnControllers)
	{
		SpawnedPawn->SpawnDefaultController();

		if (Config->IdleTaskTree)
		{
			Cast<AAIController>(SpawnedPawn->GetController())
				->GetComponentByClass<UBehaviorTreeComponent>()
				->SetDynamicSubtree(TAG_IdleTaskInjection, Config->IdleTaskTree);
		}
	}

	SpawnedPawn->OnDestroyed.AddDynamic(this, &ANPCSpawner::HandleSpawnedPawnDestroyed);
	SpawnedPawns.Add(SpawnedPawn);

	return true;
}

TSubclassOf<APawn> ANPCSpawner::PickPawnClass() const
{
	if (!Config)
	{
		return nullptr;
	}

	float TotalWeight = 0.f;

	for (const FNPCSpawnEntry& Entry : Config->SpawnEntries)
	{
		if (Entry.PawnClass && Entry.Weight > 0.f)
		{
			TotalWeight += Entry.Weight;
		}
	}

	if (TotalWeight <= 0.f)
	{
		return nullptr;
	}

	float Roll = FMath::FRandRange(0.f, TotalWeight);

	for (const FNPCSpawnEntry& Entry : Config->SpawnEntries)
	{
		if (!Entry.PawnClass || Entry.Weight <= 0.f)
		{
			continue;
		}

		Roll -= Entry.Weight;

		if (Roll <= 0.f)
		{
			return Entry.PawnClass;
		}
	}

	return nullptr;
}

FTransform ANPCSpawner::PickSpawnTransform() const
{
	TArray<USceneComponent*> ValidPoints;

	for (USceneComponent* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			ValidPoints.Add(Point);
		}
	}

	if (ValidPoints.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, ValidPoints.Num() - 1);
		return ValidPoints[Index]->GetComponentTransform();
	}

	return GetActorTransform();
}

void ANPCSpawner::CleanupDeadPawns()
{
	SpawnedPawns.RemoveAll([](const TObjectPtr<APawn>& Pawn)
		{
			return !IsValid(Pawn);
		});
}

int32 ANPCSpawner::GetAliveCount() const
{
	int32 Count = 0;

	for (APawn* Pawn : SpawnedPawns)
	{
		if (IsValid(Pawn))
		{
			++Count;
		}
	}

	return Count;
}

void ANPCSpawner::HandleSpawnedPawnDestroyed(AActor* DestroyedActor)
{
	SpawnedPawns.Remove(Cast<APawn>(DestroyedActor));
}