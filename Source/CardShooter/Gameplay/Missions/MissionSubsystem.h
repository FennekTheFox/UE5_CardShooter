//#pragma once
//
//#include "WarwolfMissionInterface.h"
//#include "Subsystems/GameInstanceSubsystem.h"
//#include "StateTreeEvents.h"
//
//#include "MissionSubsystem.generated.h"
//
//class UWarwolfMissionDataAsset;
//
//UCLASS(BlueprintType)
//class UMissionSubsystem : public UGameInstanceSubsystem,
//	public IWarwolfMissionInterface
//{
//	GENERATED_BODY()
//
//public:
//	void SelectMission_Implementation(UWarwolfMissionDataAsset* InMissionData, const bool bStartMissionInstantly) override;
//	void StartMission_Implementation() override;
//	void SendMissionEvent_Implementation(const FStateTreeEvent& Event) override;
//
//
//	UPROPERTY()
//	TObjectPtr<UWarwolfMissionDataAsset> MissionData;
//};