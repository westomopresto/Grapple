// Copyright Sam Bonifacio. All Rights Reserved.

#include "Misc/AutoSettingsConfig.h"
#include "GameFramework/InputSettings.h"
#include "AutoSettings.h"
#include "Misc/AutoSettingsError.h"

TArray<FInputActionKeyMapping> FInputMappingGroup::GetAllActions(FName ActionName, FGameplayTag KeyGroup) const
{
	const bool bAnyKeyGroup = !KeyGroup.IsValid();

	if(!bAnyKeyGroup && !GetDefault<UAutoSettingsConfig>()->IsKeyGroupDefined(KeyGroup))
	{
		FAutoSettingsError::LogError(FString::Printf(TEXT("Undefined Key Group with tag '%s'. Please add a Key Group with the tag in the Auto Settings page of the project settings."), *KeyGroup.ToString()));
	}

	TArray<FInputActionKeyMapping> Result;
	for (FInputActionKeyMapping Action : ActionMappings)
	{
		if (Action.ActionName == ActionName && (bAnyKeyGroup || GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Action.Key) == KeyGroup))
		{
			Result.Add(Action);
		}
	}
	return Result;
}

TArray<FInputAxisKeyMapping> FInputMappingGroup::GetAllAxis(FName AxisName, float Scale, FGameplayTag KeyGroup, bool bAnyScale) const
{
	const bool bAnyKeyGroup = !KeyGroup.IsValid();

	if (!bAnyKeyGroup && !GetDefault<UAutoSettingsConfig>()->IsKeyGroupDefined(KeyGroup))
	{
		FAutoSettingsError::LogError(FString::Printf(TEXT("Undefined Key Group with tag '%s'. Please add a Key Group with the tag in the Auto Settings page of the project settings."), *KeyGroup.ToString()));
	}

	TArray<FInputAxisKeyMapping> Result;
	for (FInputAxisKeyMapping Axis : AxisMappings)
	{
		if (Axis.AxisName == AxisName && (bAnyScale || Axis.Scale == Scale || GetDefault<UAutoSettingsConfig>()->IsAnalogKey(Axis.Key)) && (bAnyKeyGroup || GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Axis.Key) == KeyGroup))
		{
			Result.Add(Axis);
		}
	}
	return Result;
}

FInputActionKeyMapping FInputMappingGroup::GetAction(FName ActionName, FGameplayTag KeyGroup) const
{
	TArray<FInputActionKeyMapping> Actions = GetAllActions(ActionName, KeyGroup);
	return Actions.IsValidIndex(0) ? Actions[0] : FInputActionKeyMapping();
}

FInputAxisKeyMapping FInputMappingGroup::GetAxis(FName AxisName, float Scale, FGameplayTag KeyGroup) const
{
	TArray<FInputAxisKeyMapping> Axis = GetAllAxis(AxisName, Scale, KeyGroup);
	return Axis.IsValidIndex(0) ? Axis[0] : FInputAxisKeyMapping();
}

void FInputMappingGroup::ReplaceAction(FInputActionKeyMapping Action, bool bAnyKeyGroup)
{
	// Filter key group unless using any
	const FGameplayTag KeyGroup = bAnyKeyGroup ? FGameplayTag() : GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Action.Key);

	for (FInputActionKeyMapping& ExistingAction : GetAllActions(Action.ActionName, KeyGroup))
	{
		ActionMappings.RemoveAll([ExistingAction](FConfigActionKeyMapping Mapping) { return Mapping == ExistingAction; });
	}
	ActionMappings.Add(FConfigActionKeyMapping(Action));
}

void FInputMappingGroup::ReplaceAxis(FInputAxisKeyMapping Axis, bool bAnyKeyGroup)
{
	const bool bIsAnalog = GetDefault<UAutoSettingsConfig>()->IsAnalogKey(Axis.Key);

	// Filter key group unless using any
	const FGameplayTag KeyGroup = bAnyKeyGroup ? FGameplayTag() : GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Axis.Key);

	for (FInputAxisKeyMapping& ExistingAxis : GetAllAxis(Axis.AxisName, Axis.Scale, KeyGroup, bIsAnalog))
	{
		AxisMappings.RemoveAll([ExistingAxis](FConfigAxisKeyMapping Mapping) { return Mapping == ExistingAxis; });
	}
	AxisMappings.Add(FConfigAxisKeyMapping(Axis));
}

void FInputMappingGroup::UnbindChord(FKey Key, bool ShiftDown, bool CtrlDown, bool AltDown, bool CmdDown)
{
	// Remove all action mappings with same chord
	ActionMappings.RemoveAll([Key, ShiftDown, CtrlDown, AltDown, CmdDown](FInputActionKeyMapping Action) { return Action.Key == Key && Action.bShift == ShiftDown && Action.bCtrl == CmdDown && Action.bAlt == AltDown && Action.bCmd == CmdDown; });
	
	// Since axis cannot have modifiers, only unbind axis with same key if no modifiers are down
	if(!ShiftDown && !CtrlDown && !AltDown && !CmdDown)
		AxisMappings.RemoveAll([Key](FInputAxisKeyMapping Axis) { return Axis.Key == Key; });
}

FInputMappingPreset::FInputMappingPreset(FGameplayTag InTag, TArray<FInputActionKeyMapping> ActionMappings, TArray<FInputAxisKeyMapping> AxisMappings)
{
	PresetTag = InTag;

	SetMappings(ActionMappings, AxisMappings);
}

TArray<int32> FInputMappingPreset::GetMappingGroupsToUnbind(int32 SourceMappingGroup) const
{
	TArray<int32> Result;

	for (int i = 0; i < MappingGroups.Num(); i++)
	{
		if (GetDefault<UAutoSettingsConfig>()->ShouldBindingsBeUniqueBetweenMappingGroups(SourceMappingGroup, i))
		{
			Result.Add(i);
		}
	}

	return Result;
}

void FInputMappingPreset::SetMappings(TArray<FInputActionKeyMapping> ActionMappings, TArray<FInputAxisKeyMapping> AxisMappings)
{
	MappingGroups.Empty();

	for (const FInputActionKeyMapping Action : ActionMappings)
	{
		// Find first group without that action
		bool FoundGroup = false;
		for (FInputMappingGroup& Group : MappingGroups)
		{
			if (Group.GetAllActions(Action.ActionName, GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Action.Key)).Num() == 0)
			{
				Group.ActionMappings.Add(Action);
				FoundGroup = true;
				break;
			}
		}
		if (!FoundGroup)
		{
			MappingGroups.AddDefaulted();
			MappingGroups.Last().ActionMappings.Add(Action);
		}
	}

	for (const FInputAxisKeyMapping Axis : AxisMappings)
	{
		// Find first group without that axis
		const bool IsAnalog = GetDefault<UAutoSettingsConfig>()->IsAnalogKey(Axis.Key);

		bool FoundGroup = false;
		for (FInputMappingGroup& Group : MappingGroups)
		{
			if (Group.GetAllAxis(Axis.AxisName, Axis.Scale, GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Axis.Key), IsAnalog).Num() == 0)
			{
				Group.AxisMappings.Add(Axis);
				FoundGroup = true;
				break;
			}
		}
		if (!FoundGroup)
		{
			MappingGroups.AddDefaulted();
			MappingGroups.Last().AxisMappings.Add(Axis);
		}
	}
}

void FInputMappingPreset::ReplaceAction(const FInputActionKeyMapping& Action, int32 MappingGroup, bool bAnyKeyGroup)
{
	// Use first mapping group if none specified
	if (MappingGroup < 0)
		MappingGroup = 0;

	// Unbind key from applicable mapping groups
	if (!GetDefault<UAutoSettingsConfig>()->AllowMultipleBindingsPerKey)
	{
		UnbindChord(Action.Key, GetMappingGroupsToUnbind(MappingGroup), Action.bShift, Action.bCtrl, Action.bAlt, Action.bCmd);
	}

	if (!MappingGroups.IsValidIndex(MappingGroup))
	{
		// Add new mapping group if we need one
		MappingGroups.AddDefaulted(MappingGroup - MappingGroups.Num() + 1);
	}

	MappingGroups[MappingGroup].ReplaceAction(Action, bAnyKeyGroup);
}

void FInputMappingPreset::ReplaceAxis(const FInputAxisKeyMapping& Axis, int32 MappingGroup, bool bAnyKeyGroup)
{
	// Use first mapping group if none specified
	if (MappingGroup < 0)
		MappingGroup = 0;

	// Unbind key from applicable mapping groups
	if (!GetDefault<UAutoSettingsConfig>()->AllowMultipleBindingsPerKey)
	{
		UnbindChord(Axis.Key, GetMappingGroupsToUnbind(MappingGroup));
	}

	if (!MappingGroups.IsValidIndex(MappingGroup))
	{
		// Add new mapping group if we need one
		MappingGroups.AddDefaulted(MappingGroup - MappingGroups.Num() + 1);
	}

	MappingGroups[MappingGroup].ReplaceAxis(Axis, bAnyKeyGroup);
}

void FInputMappingPreset::UnbindChord(FKey Key, TArray<int32> MappingGroupIds, bool ShiftDown, bool CtrlDown, bool AltDown, bool CmdDown)
{
	for (int32 MappingGroupId : MappingGroupIds)
	{
		FInputMappingGroup& Group = MappingGroups[MappingGroupId];
		Group.UnbindChord(Key, ShiftDown, CtrlDown, AltDown, CmdDown);
	}
}

void FInputMappingPreset::Apply(APlayerController* Player)
{
	if (!ensure(Player && Player->PlayerInput))
	{
		return;
	}

	const UAutoSettingsConfig* Config = GetDefault<UAutoSettingsConfig>();

	// Loop through ActionMappings and remove any that aren't in Blacklist,
	// then add ones from GetAction() onto it (thus preserving any actions from the blacklist)
	Player->PlayerInput->ActionMappings.RemoveAll([Config](const FInputActionKeyMapping& Mapping)
	{
		return !Config->BlacklistedActions.Contains(Mapping.ActionName);
	});
	Player->PlayerInput->ActionMappings.Append(GetActions());

	// Same for axes
	Player->PlayerInput->AxisMappings.RemoveAll([Config](const FInputAxisKeyMapping& Mapping)
	{
		return !Config->BlacklistedAxes.Contains(Mapping.AxisName);
	});
	Player->PlayerInput->AxisMappings.Append(GetAxes());

	// Rebuild
	Player->PlayerInput->ForceRebuildingKeyMaps(false);
}

UTexture* FKeyIconSet::GetIcon(FKey Key) const
{
	const FKeyIconPair* FoundPair = Icons.FindByPredicate([Key](const FKeyIconPair KeyIconPair) {
		return KeyIconPair.Key == Key;
	});

	if (FoundPair)
	{
		UTexture* FoundIcon = FoundPair->Icon;

		if (!FoundIcon)
		{
			UE_LOG(LogAutoSettings, Warning, TEXT("FKeyIconSet::GetIcon: Key icon defined but empty - Key: %s, tags: %s"), *Key.ToString(), *Tags.ToString());
		}

		return FoundIcon;
	}

	return nullptr;
}

UAutoSettingsConfig::UAutoSettingsConfig() : bAutoInitializePlayerInputOverrides(true), AllowModifierKeys(true), AllowMultipleBindingsPerKey(false), MouseMoveCaptureDistance(20.0f)
{
	ShiftModifierOverrideText = NSLOCTEXT("AutoSettings", "ShiftKey", "Shift");
	CtrlModifierOverrideText = NSLOCTEXT("AutoSettings", "CtrlKey", "Ctrl");
	AltModifierOverrideText = NSLOCTEXT("AutoSettings", "AltKey", "Alt");
	CmdModifierOverrideText = NSLOCTEXT("AutoSettings", "CmdKey", "Cmd");
}

TArray<FInputMappingPreset> UAutoSettingsConfig::GetInputPresets() const
{
	TArray<FInputMappingPreset> Result = InputPresets;

	if (InputPresets.Num() < 1)
	{
		// Add default if no presets
		Result.Add(FInputMappingPreset(FGameplayTag::EmptyTag, true));
	}

	const TArray<FInputActionKeyMapping> DefaultActions = GetDefault<UInputSettings>()->GetActionMappings();
	const TArray<FInputAxisKeyMapping> DefaultAxis = GetDefault<UInputSettings>()->GetAxisMappings();

	for (FInputMappingPreset& Preset : Result)
	{
		// Add all default mappings first
		if (Preset.bUseDefaultMappings)
		{
			Preset.SetMappings(DefaultActions, DefaultAxis);
		}

		// Remove any key mappings with for blacklisted actions & axes
		for (FInputMappingGroup& Group : Preset.MappingGroups)
		{
			Group.ActionMappings.RemoveAll([this](const FInputActionKeyMapping& Mapping)
			{
				return BlacklistedActions.Contains(Mapping.ActionName);
			});

			Group.AxisMappings.RemoveAll([this](const FInputAxisKeyMapping& Mapping)
			{
				return BlacklistedAxes.Contains(Mapping.AxisName);
			});
		}
	}

	return Result;
}

UTexture* UAutoSettingsConfig::GetIconForKey(FKey InKey, FGameplayTagContainer Tags) const
{
	UTexture* Result;

	// First check if an icon matches input tags exactly
	for (const FKeyIconSet& Set : KeyIconSets)
	{
		if (Set.Tags == Tags)
		{
			Result = Set.GetIcon(InKey);
			if (Result)
				return Result;
		}
	}

	// Check if an icon contains all input tags
	for (const FKeyIconSet& Set : KeyIconSets)
	{
		if (Set.Tags.DoesTagContainerMatch(Tags, EGameplayTagMatchType::Explicit, EGameplayTagMatchType::Explicit, EGameplayContainerMatchType::All))
		{
			Result = Set.GetIcon(InKey);
			if (Result)
				return Result;
		}
	}

	// Check if an icon contains any of the input tags
	for (const FKeyIconSet& Set : KeyIconSets)
	{
		if (Set.Tags.DoesTagContainerMatch(Tags, EGameplayTagMatchType::Explicit, EGameplayTagMatchType::Explicit, EGameplayContainerMatchType::Any))
		{
			Result = Set.GetIcon(InKey);
			if (Result)
				return Result;
		}
	}

	// Fall back to first icon we can find
	for (const FKeyIconSet& Set : KeyIconSets)
	{
		Result = Set.GetIcon(InKey);
		if (Result)
			return Result;
	}

	return nullptr;
}

FText UAutoSettingsConfig::GetKeyFriendlyName(FKey Key) const
{
	// There seems to be some issues where setting FKey of None in BP
	// then returns a display name of "(" and stuff like that, so just manually
	// handle case of none key
	if (!Key.IsValid())
		return FText::FromString(FString(TEXT("None")));

	for (const FKeyFriendlyName& KeyFriendlyName : KeyFriendlyNames)
	{
		if (KeyFriendlyName.Key == Key)
		{
			return KeyFriendlyName.FriendlyName;
		}
	}
	return Key.GetDisplayName();
}

FText UAutoSettingsConfig::GetKeyFriendlyNameStatic(FKey Key)
{
	return GetDefault<UAutoSettingsConfig>()->GetKeyFriendlyName(Key);
}

FGameplayTag UAutoSettingsConfig::GetKeyGroupStatic(FKey Key)
{
	return GetDefault<UAutoSettingsConfig>()->GetKeyGroup(Key);
}

bool UAutoSettingsConfig::DoesKeyGroupContainKey(FGameplayTag KeyGroupTag, FKey Key) const
{
	for (FKeyGroup KeyGroup : KeyGroups)
	{
		if (KeyGroup.KeyGroupTag == KeyGroupTag)
		{
			return KeyGroup.Contains(Key);
		}
	}

	return false;
}

bool UAutoSettingsConfig::IsKeyGroupDefined(FGameplayTag KeyGroupTag) const
{
	return KeyGroups.ContainsByPredicate([KeyGroupTag](const FKeyGroup& KeyGroup) { return KeyGroup.KeyGroupTag == KeyGroupTag; });
}

FGameplayTag UAutoSettingsConfig::GetKeyGroup(FKey Key) const
{
	for (FKeyGroup KeyGroup : KeyGroups)
	{
		if (KeyGroup.Contains(Key))
		{
			return KeyGroup.KeyGroupTag;
		}
	}

	return FGameplayTag();
}

bool UAutoSettingsConfig::SameKeyGroup(FKey KeyA, FKey KeyB) const
{
	return GetKeyGroup(KeyA) == GetKeyGroup(KeyB);
}

FKeyScale UAutoSettingsConfig::GetAnalogKey(FKey InBooleanKey) const
{
	for (FAxisAssociation AxisAssociation : AxisAssociations)
	{
		for (FKeyScale BooleanKey : AxisAssociation.BooleanKeys)
		{
			if (BooleanKey.Key == InBooleanKey)
			{
				return FKeyScale(AxisAssociation.AnalogKey, BooleanKey.Scale);
			}
		}
	}

	return FKeyScale();
}

bool UAutoSettingsConfig::IsAnalogKey(FKey Key) const
{
	const FAxisAssociation* FoundAssociation = AxisAssociations.FindByPredicate([Key](const FAxisAssociation Association) {
		return Association.AnalogKey == Key;
	});

	return FoundAssociation != nullptr;
}

bool UAutoSettingsConfig::IsKeyAllowed(FKey Key) const
{
	if (AllowedKeys.Num() > 0)
	{
		bool HasAllowedKey = false;

		for (FKey AllowedKey : AllowedKeys)
		{
			if (Key == AllowedKey)
			{
				HasAllowedKey = true;
				break;
			}
		}

		if (!HasAllowedKey)
		{
			// Whitelist populated but doesn't have key, therefore key is disallowed
			return false;
		}
	}

	// Disallow if key on blacklist

	for (FKey DisallowedKey : DisallowedKeys)
	{
		if (Key == DisallowedKey)
		{
			return false;
		}
	}

	// Passed both whitelist and blacklist, key is allowed
	return true;
}

bool UAutoSettingsConfig::ShouldBindingsBeUniqueBetweenMappingGroups(int32 MappingGroupA, int32 MappingGroupB) const
{
	// Always enforce unique if not allowing multiple bindings per key
	if (!AllowMultipleBindingsPerKey)
		return true;

	if (MappingGroupA == MappingGroupB)
		return true;

	for (const FMappingGroupLink& Link : MappingGroupLinks)
	{
		if (Link.MappingGroups.Contains(MappingGroupA) && Link.MappingGroups.Contains(MappingGroupB))
		{
			// Both mapping groups are linked and should have unique bindings
			return true;
		}
	}

	return false;
}