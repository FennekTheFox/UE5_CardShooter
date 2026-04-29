// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
//
#include "WarwolfMissionDataAsset.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfMissionInterface.h"
#include "WarwolfMissionUiInterface.h"
#include "Tasks/WarwolfMissionTaskInterface.h"
//
#include "WarwolfExampleGameState.generated.h"

class UWarwolfMissionManager;

DECLARE_LOG_CATEGORY_EXTERN(LogWarwolfGameState, Log, All);

/**
 * 
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API AWarwolfExampleGameState : public AGameStateBase, public IWarwolfMissionInterface, public IWarwolfMissionUiInterface, public IWarwolfMissionTaskInterface
{
	GENERATED_BODY()

	AWarwolfExampleGameState();

	
public:

	// @brief Mission state tree component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UWarwolfMissionManager> MissionManager = nullptr;


protected:

	// IWarwolfMissionInterface
	virtual void StartMission_Implementation() override;
	virtual void SelectMission_Implementation(UWarwolfMissionDataAsset* InMissionData, const bool bStartMissionInstantly) override;
	virtual void SendMissionEvent_Implementation(const FStateTreeEvent& Event) override;
	// ~IWarwolfMissionInterface

	// IWarwolfMissionUiInterface
	virtual void ToastMission_Implementation(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType) override;
	virtual void ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType) override;
	virtual void ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid) override;
	virtual void HideObjective_Implementation(const FGuid InGuid) override;
	virtual void HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid) override;
	virtual void UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount) override;
	virtual void UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime) override;
	// ~IWarwolfMissionUiInterface

	// IWarwolfMissionTaskInterface
	virtual ULevelSequencePlayer* PlayLevelSequence_Implementation(ULevelSequence* InLevelSequence) override;
	FOnWarwolfIntCount& GetIntCountDelegate() override; // Returns a null delegate since we don't need this, but we can't build without it.
	// ~IWarwolfMissionTaskInterface

	// @brief Replicate toasts to clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ToastMission(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType);

	// @brief Replicate show objective to clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowObjective(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType);

	
	// @brief Replicate show sub-objective to clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowSubObjective(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid);

	
	// @brief Replicate hide objective to clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HideObjective(const FGuid InGuid);

	// @brief Replicate hide sub-objective to clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HideSubObjective(const FGuid InObjGuid, const FGuid InGuid);
	
	// @brief Replicate update intcount objective to clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateSubObjective_IntCount(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount);

	// @brief Replicate update timer objective to clients.
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateSubObjective_Timer(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime);

	// @brief Replicate mission cinematic.
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayLevelSequence(ULevelSequence* InLevelSequence);

	// @brief Play a level sequence
	UFUNCTION()
	ULevelSequencePlayer* PlayLevelSequence_Internal(ULevelSequence* InLevelSequence) const;

	private:

	// @brief Mission we currently have selected.
	UPROPERTY(Transient)
	TObjectPtr<UWarwolfMissionDataAsset> SelectedMission = nullptr;
	
};
