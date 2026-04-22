#pragma once

#include "CoreMinimal.h"

#include "ProjectPlayerController.generated.h"

class UProjectAbilitySystemComponent;

UCLASS(BlueprintType, Blueprintable)
class PROJECTABILITYSYSTEM_PLUGIN_API AProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//BEGIN APlayerController
	void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//END APlayerController

	UProjectAbilitySystemComponent* GetProjectAbilitySystemComponent();
};
