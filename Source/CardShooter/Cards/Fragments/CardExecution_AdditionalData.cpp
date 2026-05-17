#include "CardExecution_AdditionalData.h"

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
	}

	ensure(false);
	return FText::GetEmpty();
}