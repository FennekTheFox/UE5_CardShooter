// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/UI/WarwolfEMissionWidget.h"
#include "Example/UI/WarwolfEMissionToastWidget.h"
#include "Example/UI/WarwolfEMissionSubObjWidget.h"
#include "Components/VerticalBox.h"

void UWarwolfExampleMissionWidget::ShowMissionToast(const FText InToastName, const FText InMissionName, const EWarwolfMissionType InMissionType)
{
	if(!IsValid(ToastWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s MissionWidget is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	ToastWidget->ShowMissionToast(InToastName, InMissionName, InMissionType);
}

void UWarwolfExampleMissionWidget::ShowObjective_Implementation(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType)
{
	if (!ContainerWidget) return;

	UVerticalBox* Container = ContainerWidget->ObjectiveList;
	if (!Container)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Container is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	UWarwolfExampleMissionObjectiveWidget* WidgetInstance = CreateWidget<UWarwolfExampleMissionObjectiveWidget>(GetWorld(), ObjectiveWidgetClass);
	if(!WidgetInstance)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s WidgetInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	WidgetInstance->SetGuid(InGuid);
	WidgetInstance->SetObjectiveName(InObjText);
	WidgetInstance->SetObjectiveType(InMissionType);
	Container->AddChild(WidgetInstance);
}

void UWarwolfExampleMissionWidget::HideObjective_Implementation(const FGuid InGuid)
{
	if (!ContainerWidget) return;

	UVerticalBox* Container = ContainerWidget->ObjectiveList;
	if (!Container)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Container is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	TArray<UWidget*> Children = Container->GetAllChildren();
	for (UWidget* Widget : Children)
	{
		UWarwolfExampleMissionObjectiveWidget* ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!ObjWidget || InGuid != ObjWidget->GetGuid()) continue;
		
		ObjWidget->HideObjective();
		return;
	}

	UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InGuid.ToString());
}

void UWarwolfExampleMissionWidget::ShowSubObjective_Implementation(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid)
{
	if (!ContainerWidget) return;
	
	UVerticalBox* Container = ContainerWidget->ObjectiveList;
	if (!Container)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Container is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	UWarwolfExampleMissionObjectiveWidget* ObjWidget = nullptr;
	TArray<UWidget*> Children = Container->GetAllChildren();
	bool bFoundObjWidget = false;

	for (UWidget* Widget : Children)
	{
		ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!IsValid(ObjWidget) || InObjGuid != ObjWidget->GetGuid()) continue;
		
		bFoundObjWidget = true;
		break;
	}

	if (!bFoundObjWidget || !IsValid(ObjWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	UWarwolfExampleMissionSubObjWidget* WidgetInstance = CreateWidget<UWarwolfExampleMissionSubObjWidget>(GetWorld(), SubObjectiveWidgetClass);
	if(!IsValid(WidgetInstance))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s WidgetInstance is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	WidgetInstance->SetGuid(InGuid);
	WidgetInstance->SetSubObjectiveText(InSubObjText);
	WidgetInstance->PlayAppearanceAnim();
	
	ObjWidget->SubObjContainer->AddChild(WidgetInstance);
}

void UWarwolfExampleMissionWidget::HideSubObjective_Implementation(const FGuid InObjGuid, const FGuid InGuid)
{
	if (!ContainerWidget) return;
	
	UVerticalBox* Container = ContainerWidget->ObjectiveList;
	if (!Container)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Container is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	UWarwolfExampleMissionObjectiveWidget* ObjWidget = nullptr;
	TArray<UWidget*> Children = Container->GetAllChildren();
	bool bFoundObjWidget = false;

	for (UWidget* Widget : Children)
	{
		ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!ObjWidget || InObjGuid != ObjWidget->GetGuid()) continue;
		
		bFoundObjWidget = true;
		break;
	}

	if (!bFoundObjWidget || !IsValid(ObjWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	UWarwolfExampleMissionSubObjWidget* SubObjWidget = nullptr;
	TArray<UWarwolfExampleMissionSubObjWidget*> SubObjChildren = ObjWidget->GetAllSubObjectives();
	bool bFoundSubObjWidget = false;
	for (UWarwolfExampleMissionSubObjWidget* Widget : SubObjChildren)
	{
		SubObjWidget = Cast<UWarwolfExampleMissionSubObjWidget>(Widget);

		if (!SubObjWidget || InGuid != SubObjWidget->GetGuid()) continue;
		
		bFoundSubObjWidget = true;
		break;
	}

	if (!bFoundSubObjWidget || !SubObjWidget)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find a sub-objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	SubObjWidget->HideSubObjective();
}

void UWarwolfExampleMissionWidget::UpdateSubObjective_IntCount_Implementation(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount)
{
	if (!ContainerWidget) return;
	
	UVerticalBox* Container = ContainerWidget->ObjectiveList;
	if (!Container)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Container is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	UWarwolfExampleMissionObjectiveWidget* ObjWidget = nullptr;
	TArray<UWidget*> Children = Container->GetAllChildren();
	bool bFoundObjWidget = false;

	for (UWidget* Widget : Children)
	{
		ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!IsValid(ObjWidget) || InObjGuid != ObjWidget->GetGuid()) continue;
		
		bFoundObjWidget = true;
		break;
	}

	if (!bFoundObjWidget || !IsValid(ObjWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	UWarwolfExampleMissionSubObjWidget* SubObjWidget = nullptr;
	TArray<UWarwolfExampleMissionSubObjWidget*> SubObjChildren = ObjWidget->GetAllSubObjectives();
	bool bFoundSubObjWidget = false;
	for (UWarwolfExampleMissionSubObjWidget* Widget : SubObjChildren)
	{
		SubObjWidget = Cast<UWarwolfExampleMissionSubObjWidget>(Widget);

		if (!IsValid(SubObjWidget) || InGuid != SubObjWidget->GetGuid()) continue;
		
		bFoundSubObjWidget = true;
		break;
	}

	if (!bFoundSubObjWidget || !IsValid(SubObjWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find a sub-objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	SubObjWidget->UpdateCount(InCurrentCount, InTotalCount);
}

void UWarwolfExampleMissionWidget::UpdateSubObjective_Timer_Implementation(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime)
{
	if (!ContainerWidget) return;
	
	UVerticalBox* Container = ContainerWidget->ObjectiveList;
	if (!Container)
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s Container is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	UWarwolfExampleMissionObjectiveWidget* ObjWidget = nullptr;
	TArray<UWidget*> Children = Container->GetAllChildren();
	bool bFoundObjWidget = false;

	for (UWidget* Widget : Children)
	{
		ObjWidget = Cast<UWarwolfExampleMissionObjectiveWidget>(Widget);

		if (!IsValid(ObjWidget) || InObjGuid != ObjWidget->GetGuid()) continue;
		
		bFoundObjWidget = true;
		break;
	}

	if (!bFoundObjWidget || !IsValid(ObjWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find an objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	UWarwolfExampleMissionSubObjWidget* SubObjWidget = nullptr;
	TArray<UWarwolfExampleMissionSubObjWidget*> SubObjChildren = ObjWidget->GetAllSubObjectives();
	bool bFoundSubObjWidget = false;
	for (UWarwolfExampleMissionSubObjWidget* Widget : SubObjChildren)
	{
		SubObjWidget = Cast<UWarwolfExampleMissionSubObjWidget>(Widget);

		if (!IsValid(SubObjWidget) || InGuid != SubObjWidget->GetGuid()) continue;
		
		bFoundSubObjWidget = true;
		break;
	}

	if (!bFoundSubObjWidget || !IsValid(SubObjWidget))
	{
		UE_LOG(LogWarwolfMissionUI, Error, TEXT("%s We didn't find a sub-objective widget with guid %s. Returning."), *FString(__FUNCTION__), *InObjGuid.ToString());
		return;
	}

	SubObjWidget->UpdateCount(InCurrentTime, InTotalTime);
}
