// Copyright Sam Bonifacio. All Rights Reserved.

#include "InputMapping/UI/AxisMapping.h"
#include "InputMapping/InputMappingManager.h"

void UAxisMapping::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAxisMapping::UpdateLabel_Implementation()
{
	GetAxisLabel()->AxisName = AxisName;
	GetAxisLabel()->Scale = Scale;
	GetAxisLabel()->MappingGroup = MappingGroup;
	GetAxisLabel()->KeyGroup = KeyGroup;
	GetAxisLabel()->bUsePlayerKeyGroup = false;
	GetAxisLabel()->IconTags = IconTags;
	GetAxisLabel()->InitializeLabel();
}

void UAxisMapping::BindChord(FInputChord InChord)
{
	FKey Key = InChord.Key;

	FKeyScale AnalogKeyScale = GetDefault<UAutoSettingsConfig>()->GetAnalogKey(Key);
	const bool UseAnalogKey = AnalogKeyScale.Key.IsValid();

	const FKey FinalKey = UseAnalogKey ? AnalogKeyScale.Key : Key;
	const float FinalScale = UseAnalogKey ? AnalogKeyScale.Scale * Scale : Scale;

	const FInputAxisKeyMapping NewMapping = FInputAxisKeyMapping(AxisName, FinalKey, FinalScale);

	// Override all key groups if none is specified
	const bool bAnyKeyGroup = !KeyGroup.IsValid();

	UInputMappingManager::Get()->AddPlayerAxisOverride(GetOwningPlayer(), NewMapping, MappingGroup, bAnyKeyGroup);
}
