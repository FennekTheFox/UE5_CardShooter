#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ProjectAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECTABILITYSYSTEM_PLUGIN_API UProjectAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
};