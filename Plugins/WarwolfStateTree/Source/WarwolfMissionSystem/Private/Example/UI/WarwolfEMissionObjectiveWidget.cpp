// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "Example/UI/WarwolfEMissionObjectiveWidget.h"

#include "WarwolfMissionTypes.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "WarwolfMissionTypes.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Example/UI/WarwolfEMissionSubObjWidget.h"

void UWarwolfExampleMissionObjectiveWidget::HideObjective()
{
	RemoveFromParent();
}

void UWarwolfExampleMissionObjectiveWidget::SetObjectiveName(const FText InObjectiveText)
{
	if (!ObjectiveNameText) return;

	ObjectiveNameText->SetText(InObjectiveText);
}

TArray<UWarwolfExampleMissionSubObjWidget*> UWarwolfExampleMissionObjectiveWidget::GetAllSubObjectives()
{
	if (!SubObjContainer) return TArray<UWarwolfExampleMissionSubObjWidget*>();

	TArray<UWidget*> Children = SubObjContainer->GetAllChildren();
	TArray<UWarwolfExampleMissionSubObjWidget*> OutChildren;
	for (UWidget* Widget : Children)
	{
		UWarwolfExampleMissionSubObjWidget* SubObjWidget = Cast<UWarwolfExampleMissionSubObjWidget>(Widget);

		if (!SubObjWidget) continue;

		OutChildren.Emplace(SubObjWidget);
	}

	return OutChildren;
}

void UWarwolfExampleMissionObjectiveWidget::SetObjectiveType(const EWarwolfMissionType InObjectiveType)
{
	if (!ObjMark) return;

	FLinearColor BrushColour = FLinearColor::White;
	switch (InObjectiveType)
	{
	case EWarwolfMissionType::Mission:
		BrushColour = FLinearColor::Red;
		break;

	case EWarwolfMissionType::Primary:
		BrushColour = FLinearColor(1.0f, 0.986698f, 0.065688f); // Yellow
		break;

	case EWarwolfMissionType::Optional:
		BrushColour = FLinearColor(0.0f, 0.14586f, 1.0f); // Blue
		break;

	default:
		BrushColour = FLinearColor::White;
		break;
	}

	ObjMark->SetBrushColor(BrushColour);
}

