#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCSpawner.generated.h"

class UNPCSpawnerConfig;
class UNPCSpawnerTrigger;
class USceneComponent;
class UArrowComponent;
class APawn;

UCLASS()
class ANPCSpawner : public AActor
{
	GENERATED_BODY()

public:
	ANPCSpawner();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "NPC Spawner")
	bool RequestSpawn();

	UFUNCTION(BlueprintCallable, Category = "NPC Spawner")
	int32 GetAliveCount() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> DefaultSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TObjectPtr<UNPCSpawnerConfig> Config;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TArray<TObjectPtr<USceneComponent>> SpawnPoints;

private:
	UPROPERTY()
	TArray<TObjectPtr<APawn>> SpawnedPawns;

	UPROPERTY()
	TArray<TObjectPtr<UNPCSpawnerTrigger>> RuntimeTriggers;

	float LastSpawnTime = -BIG_NUMBER;

	bool SpawnOne();
	TSubclassOf<APawn> PickPawnClass() const;
	FTransform PickSpawnTransform() const;
	void CleanupDeadPawns();

	UFUNCTION()
	void HandleSpawnedPawnDestroyed(AActor* DestroyedActor);
};