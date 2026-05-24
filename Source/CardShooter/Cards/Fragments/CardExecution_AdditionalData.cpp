#include "CardExecution_AdditionalData.h"

#define LOCTEXT_NAMESPACE "CardEffects"

FText FSetByCallerMetaData::GetDisplayText() const
{
	switch (DisplayType)
	{
	case ESetByCallerMagnitudeDisplayType::Flat:
		return FText::AsNumber(Magnitude);
	case ESetByCallerMagnitudeDisplayType::Percent:
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(100.f * Magnitude));
	case ESetByCallerMagnitudeDisplayType::PercentOneMinus:
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(100.f * (1.f - Magnitude)));
	case ESetByCallerMagnitudeDisplayType::Absolute:
		return FText::Format(FText::FromString("{0}"), FText::AsNumber(FMath::Abs(Magnitude)));
	case ESetByCallerMagnitudeDisplayType::AbsolutePercent:
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(FMath::Abs(100.f * Magnitude)));
	case ESetByCallerMagnitudeDisplayType::AbsolutePercentOneOver:
		return FText::Format(FText::FromString("{0}%"), FText::AsNumber(1.f / FMath::Abs(100.f * Magnitude)));
	case ESetByCallerMagnitudeDisplayType::XOver100:
		return FText::Format(FText::FromString("{0}"), FText::AsNumber(Magnitude / 100.f));
	case ESetByCallerMagnitudeDisplayType::Rotation:
	{
		float EffectiveMagnitude = ((int32)(Magnitude + 180.f) % 360) - 180.f;
		if (FMath::IsNearlyEqual(EffectiveMagnitude, 0.f)) return LOCTEXT("SetByCallerRotation_Forward", "forward");
		if (FMath::IsNearlyEqual(EffectiveMagnitude, 90.f)) return LOCTEXT("SetByCallerRotation_Right", "to the right");
		if (FMath::IsNearlyEqual(FMath::Abs(EffectiveMagnitude), 180.f)) return LOCTEXT("SetByCallerRotation_Backward", "backward");
		if (FMath::IsNearlyEqual(EffectiveMagnitude, -90.f)) return LOCTEXT("SetByCallerRotation_Left", "to the left");
		if (EffectiveMagnitude < 0.f) return FText::Format(LOCTEXT("SetByCallerRotation_DegLeft", " {0}° to the left"), FText::AsNumber(FMath::Abs(EffectiveMagnitude)));
		if (EffectiveMagnitude > 0.f) return FText::Format(LOCTEXT("SetByCallerRotation_DegRight", " {0}° to the right"), FText::AsNumber(EffectiveMagnitude));
	}
	}

	ensure(false);
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE