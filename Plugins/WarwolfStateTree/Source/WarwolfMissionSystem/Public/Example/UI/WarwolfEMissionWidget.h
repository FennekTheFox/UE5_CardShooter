// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarwolfEMissionContainerWidget.h"
#include "Blueprint/UserWidget.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfMissionUiInterface.h"
#include "WarwolfMissionTypes.h"
#include "Example/UI/WarwolfEMissionObjectiveWidget.h"
#include "Example/UI/WarwolfEMissionSubObjWidget.h"
#include "WarwolfEMissionWidget.generated.h"

class UWarwolfExampleMissionToastWidget;

/**
 * Widget containing all mission related UI.
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfExampleMissionWidget : public UUserWidget, public IWarwolfMissionUiInterface
{
	GENERATED_BODY()

	// IWarwolfMissionUiInterface
	virtual void ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType) override;
	virtual void ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid) override;
	virtual void HideObjective_Implementation(const FGuid InGuid) override;
	virtual void HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid) override;
	virtual void UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount) override;
	virtual void UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime) override;
	// ~IWarwolfMissionUiInterface

public:

	// @brief Show a toast on the mission UI.
	UFUNCTION(BlueprintCallable, Category = "WarwolfMissionSystem")
	void ShowMissionToast(const FText InToastName, const FText InMissionName, const EWarwolfMissionType InMissionType);

	// @brief Blueprint class of the objective widget we want to use.
	UPROPERTY(EditDefaultsOnly, Category = "WarwolfMissionSystem")
	TSubclassOf<UWarwolfExampleMissionObjectiveWidget> ObjectiveWidgetClass = UWarwolfExampleMissionObjectiveWidget::StaticClass();

	// @brief Blueprint class of the sub-objective widget we want to use.
	UPROPERTY(EditDefaultsOnly, Category = "WarwolfMissionSystem")
	TSubclassOf<UWarwolfExampleMissionSubObjWidget> SubObjectiveWidgetClass = UWarwolfExampleMissionSubObjWidget::StaticClass();
	

	// Widgets

	// @brief Widget that controls the toast UI.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UWarwolfExampleMissionToastWidget> ToastWidget;

	// @brief Container for all objectives for this mission.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UWarwolfExampleMissionContainerWidget> ContainerWidget;

	// ~Widgets
	
};
