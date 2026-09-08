#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NeonGameState.generated.h"

// Individual player hit event stored on server for rolling window calculations
struct FNeonDamageHitRecord
{
	float Timestamp = 0.f;
	float DamageAmount = 0.f;
};

// Internal combat state tracked per player on server
struct FNeonPlayerCombatTracker
{
	TArray<FNeonDamageHitRecord> HitHistory;
	float TotalDamage = 0.f;
	float LastDamageTimestamp = 0.f;
	float CalculatedDPS = 0.f;
};

// Replicated struct sent to clients
USTRUCT(BlueprintType)
struct FNeonPlayerDPSData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float CurrentDPS = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TotalDamage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TimeSinceLastHit = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartyDPSUpdated, const TArray<FNeonPlayerDPSData>&, PartyDPSData);

UCLASS()
class ANeonGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANeonGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "Neon|DPS")
	FOnPartyDPSUpdated OnPartyDPSUpdated;

	// [Server Only] Call this whenever a player deals damage
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Neon|DPS")
	void Server_ReportDamage(APlayerState* Attacker, float DamageAmount);

	// Reset combat session manually or after timeout
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Neon|DPS")
	void Server_ResetPlayerDPS(APlayerState* TargetPlayer);

	UFUNCTION(BlueprintPure, Category = "Neon|DPS")
	const TArray<FNeonPlayerDPSData>& GetPartyDPSData() const { return PartyDPSData; }

protected:
	// Combat session timeout duration in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Neon|DPS Config")
	float CombatTimeoutDuration = 10.0f;

	// Rolling window duration used for average DPS math (e.g. last 5 seconds of hits)
	UPROPERTY(EditDefaultsOnly, Category = "Neon|DPS Config")
	float DPSWindowDuration = 5.0f;

	// Replicated stats array for all clients
	UPROPERTY(ReplicatedUsing = OnRep_PartyDPSData)
	TArray<FNeonPlayerDPSData> PartyDPSData;

	UFUNCTION()
	void OnRep_PartyDPSData();

private:
	// Server-only raw combat history per PlayerState pointer
	TMap<TWeakObjectPtr<APlayerState>, FNeonPlayerCombatTracker> ServerCombatMap;

	void ProcessDPSCalculations(float CurrentTime);
	void SyncReplicatedData(float CurrentTime);
};