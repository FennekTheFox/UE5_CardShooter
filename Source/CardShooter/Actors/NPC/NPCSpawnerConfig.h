#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NPCSpawnerConfig.generated.h"

class APawn;
class ANPCSpawner;
class UNPCSpawnerTrigger;

USTRUCT(BlueprintType)
struct FNPCSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float Weight = 1.f;
};

UCLASS(BlueprintType)
class UNPCSpawnerConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<FNPCSpawnEntry> SpawnEntries;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "1"))
	int32 MaxAlive = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 SpawnCountPerTrigger = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0.0"))
	float SpawnCooldown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bSpawnControllers = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bDestroySpawnedOnSpawnerDestroyed = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<class UBehaviorTree> IdleTaskTree;


	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Triggers")
	TArray<TObjectPtr<UNPCSpawnerTrigger>> Triggers;


};