#include "ProjectPlayerController.h"
#include "ProjectAbilitySystemComponent.h"

void AProjectPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UProjectAbilitySystemComponent* ProjectAbilitySystemComponent = GetProjectAbilitySystemComponent())
	{
		ProjectAbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UProjectAbilitySystemComponent* AProjectPlayerController::GetProjectAbilitySystemComponent()
{
	return GetPawn() ? GetPawn()->FindComponentByClass<UProjectAbilitySystemComponent>() : nullptr;
}

