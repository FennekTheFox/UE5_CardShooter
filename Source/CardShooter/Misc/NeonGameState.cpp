#include "NeonGameState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

ANeonGameState::ANeonGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	bReplicates = true;
}

void ANeonGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANeonGameState, PartyDPSData);
}

void ANeonGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		ProcessDPSCalculations(CurrentTime);
		SyncReplicatedData(CurrentTime);
	}
}

void ANeonGameState::Server_ReportDamage(APlayerState* Attacker, float DamageAmount)
{
	if (!HasAuthority() || !Attacker || DamageAmount <= 0.f)
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	FNeonPlayerCombatTracker& Tracker = ServerCombatMap.FindOrAdd(Attacker);

	// Automatic Reset: If 10+ seconds passed since last hit, start fresh
	if (Tracker.LastDamageTimestamp > 0.f && (CurrentTime - Tracker.LastDamageTimestamp) >= CombatTimeoutDuration)
	{
		Tracker.HitHistory.Empty();
		Tracker.TotalDamage = 0.f;
		Tracker.CalculatedDPS = 0.f;
	}

	// Add hit record and aggregate total damage
	Tracker.HitHistory.Add({ CurrentTime, DamageAmount });
	Tracker.TotalDamage += DamageAmount;
	Tracker.LastDamageTimestamp = CurrentTime;
}

void ANeonGameState::Server_ResetPlayerDPS(APlayerState* TargetPlayer)
{
	if (!HasAuthority() || !TargetPlayer) return;

	ServerCombatMap.Remove(TargetPlayer);

	PartyDPSData.RemoveAll([TargetPlayer](const FNeonPlayerDPSData& Data)
		{
			return Data.PlayerState == TargetPlayer;
		});

	if (GetNetMode() != NM_DedicatedServer)
	{
		OnPartyDPSUpdated.Broadcast(PartyDPSData);
	}
}

void ANeonGameState::ProcessDPSCalculations(float CurrentTime)
{
	TArray<TWeakObjectPtr<APlayerState>> KeysToRemove;

	for (auto& [WeakPlayer, Tracker] : ServerCombatMap)
	{
		if (!WeakPlayer.IsValid())
		{
			KeysToRemove.Add(WeakPlayer);
			continue;
		}

		// Check for 10-second inactivity timeout
		if (Tracker.LastDamageTimestamp > 0.f && (CurrentTime - Tracker.LastDamageTimestamp) >= CombatTimeoutDuration)
		{
			Tracker.HitHistory.Empty();
			Tracker.TotalDamage = 0.f;
			Tracker.CalculatedDPS = 0.f;
			continue;
		}

		// Purge old hits outside the rolling DPS window
		const float WindowStartTime = CurrentTime - DPSWindowDuration;
		Tracker.HitHistory.RemoveAll([WindowStartTime](const FNeonDamageHitRecord& Record)
			{
				return Record.Timestamp < WindowStartTime;
			});

		// Calculate rolling average DPS
		float WindowDamageSum = 0.f;
		for (const FNeonDamageHitRecord& Record : Tracker.HitHistory)
		{
			WindowDamageSum += Record.DamageAmount;
		}

		Tracker.CalculatedDPS = WindowDamageSum / DPSWindowDuration;
	}

	for (const auto& Key : KeysToRemove)
	{
		ServerCombatMap.Remove(Key);
	}
}

void ANeonGameState::SyncReplicatedData(float CurrentTime)
{
	TArray<FNeonPlayerDPSData> UpdatedPartyData;

	for (auto& [WeakPlayer, Tracker] : ServerCombatMap)
	{
		if (APlayerState* PS = WeakPlayer.Get())
		{
			FNeonPlayerDPSData Entry;
			Entry.PlayerState = PS;
			Entry.CurrentDPS = Tracker.CalculatedDPS;
			Entry.TotalDamage = Tracker.TotalDamage;
			Entry.TimeSinceLastHit = (Tracker.LastDamageTimestamp > 0.f) ? (CurrentTime - Tracker.LastDamageTimestamp) : 0.f;

			UpdatedPartyData.Add(Entry);
		}
	}

	PartyDPSData = MoveTemp(UpdatedPartyData);

	if (GetNetMode() != NM_DedicatedServer)
	{
		OnPartyDPSUpdated.Broadcast(PartyDPSData);
	}
}

void ANeonGameState::OnRep_PartyDPSData()
{
	OnPartyDPSUpdated.Broadcast(PartyDPSData);
}