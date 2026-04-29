// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/UI/WarwolfEMissionContainerWidget.h"

#include "Components/VerticalBox.h"
#include "Example/UI/WarwolfEMissionObjectiveWidget.h"
#include "Example/UI/WarwolfEMissionSubObjWidget.h"

UWarwolfExampleMissionObjectiveWidget* UWarwolfExampleMissionContainerWidget::GetObjectiveWithGuid(const FGuid InGuid) const
{
	if (!ObjectiveList) return nullptr;

	TArray<UWidget*> Children = ObjectiveList->GetAllChildren();

	for (UWidget* Widget : Children)
	{
		UWarwolfExampleMissionObjectiveWidget* ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!ObjWidget || InGuid != ObjWidget->GetGuid()) continue;
		
		return ObjWidget;
	}

	UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning none."), *FString(__FUNCTION__), *InGuid.ToString());
	return nullptr;
}

UWarwolfExampleMissionSubObjWidget* UWarwolfExampleMissionContainerWidget::GetSubObjectiveWithGuid(const FGuid ObjectiveGuid, const FGuid SubObjectiveGuid) const
{
	if (!ObjectiveList) return nullptr;

	UWarwolfExampleMissionObjectiveWidget* ObjWidget = nullptr;
	TArray<UWidget*> Children = ObjectiveList->GetAllChildren();
	bool bFoundObjWidget = false;

	for (UWidget* Widget : Children)
	{
		ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!ObjWidget || ObjectiveGuid != ObjWidget->GetGuid()) continue;
		
		bFoundObjWidget = true;
	}

	if (!bFoundObjWidget || !ObjWidget)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning none."), *FString(__FUNCTION__), *ObjectiveGuid.ToString());
		return nullptr;
	}

	TArray<UWarwolfExampleMissionSubObjWidget*> SubObjWidgets = ObjWidget->GetAllSubObjectives();
	for(UWarwolfExampleMissionSubObjWidget* Widget : SubObjWidgets)
	{
		if (!Widget || Widget->GetGuid() != SubObjectiveGuid) continue;

		return Widget;
	}

	UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find a sub-objective widget with guid %s. Returning none."), *FString(__FUNCTION__), *SubObjectiveGuid.ToString());
	return nullptr;
}

