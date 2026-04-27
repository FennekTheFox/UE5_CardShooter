#pragma once

#include "CardAttributeFragment_Base.h"

#include "CardEffectFragment_Echo.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Echo"))
class UCardEffectFragment_Echo : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 EchoCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float EchoDelay;
};