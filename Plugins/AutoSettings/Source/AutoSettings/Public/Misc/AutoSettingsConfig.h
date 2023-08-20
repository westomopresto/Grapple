// Copyright Sam Bonifacio. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "AutoSettingsConfig.generated.h"

// Sadly the mapping names aren't editable in config unless using these child classes

// Config editable action mapping
USTRUCT(BlueprintType)
struct FConfigActionKeyMapping : public FInputActionKeyMapping
{
	GENERATED_USTRUCT_BODY()

	FConfigActionKeyMapping() {}

	FConfigActionKeyMapping(FInputActionKeyMapping Base)
	{
		ActionName = Base.ActionName;
		Key = Base.Key;
		bShift = Base.bShift;
		bCtrl = Base.bCtrl;
		bAlt = Base.bAlt;
		bCmd = Base.bCmd;
	}

	FString ToDebugString() const
	{
		return FString::Printf(TEXT("ActionName: %s, Key: %s, bShift: %i, bCtrl: %i, bAlt: %i, bCmd: %i"),
			*ActionName.ToString(),
			*Key.ToString(),
			(int32)bShift,
			(int32)bCtrl,
			(int32)bAlt,
			(int32)bCmd);
	}
};

// Config editable axis mapping
USTRUCT(BlueprintType)
struct FConfigAxisKeyMapping : public FInputAxisKeyMapping
{
	GENERATED_USTRUCT_BODY()

	FConfigAxisKeyMapping() {}

	FConfigAxisKeyMapping(FInputAxisKeyMapping Base)
	{
		AxisName = Base.AxisName;
		Key = Base.Key;
		Scale = Base.Scale;
	}

	FString ToDebugString() const
	{
		return FString::Printf(TEXT("AxisName: %s, Key: %s, Scale: %f"),
			*AxisName.ToString(),
			*Key.ToString(),
			Scale);
	}
};

// An input mapping group represents a set of mappings for which each action or axis has a single binding
// Each action should have a unique name and each axis should have a unique name + scale combination, however analog axis keys count as all scales
// It's valid to have axis mappings for (Name: MoveForward, Scale: 1, Key: W) and (Name: MoveForward, Scale: -1, Key: S) at the same time
// It's not valid to have (Name: MoveForward, Scale: 1, Key: GamepadLeftStickY) and (Name: MoveForward, Scale: -1, Key: S)
USTRUCT()
struct FInputMappingGroup
{
	GENERATED_USTRUCT_BODY()

	// Action mappings in this group
	UPROPERTY(config, EditAnywhere, Category = "Input Mapping Group", meta = (TitleProperty = "ActionName"))
	TArray<FConfigActionKeyMapping> ActionMappings;

	// Axis mappings in this group
	UPROPERTY(config, EditAnywhere, Category = "Input Mapping Group", meta = (TitleProperty = "AxisName"))
	TArray<FConfigAxisKeyMapping> AxisMappings;

	// Return all action mappings that match the given parameters
	TArray<FInputActionKeyMapping> GetAllActions(FName ActionName, FGameplayTag KeyGroup) const;

	// Return all axis mappings that match the given parameters
	TArray<FInputAxisKeyMapping> GetAllAxis(FName AxisName, float Scale, FGameplayTag KeyGroup, bool bAnyScale = false) const;

	// Returns the first action that matches the given parameters
	FInputActionKeyMapping GetAction(FName ActionName, FGameplayTag KeyGroup) const;

	// Returns the first axis that matches the given parameters
	FInputAxisKeyMapping GetAxis(FName AxisName, float Scale, FGameplayTag KeyGroup) const;

	// Add the given action and remove any existing actions that it should replace
	void ReplaceAction(FInputActionKeyMapping Action, bool bAnyKeyGroup = false);

	// Add the given axis and remove any exist axes that it should replace
	void ReplaceAxis(FInputAxisKeyMapping Axis, bool bAnyKeyGroup = false);

	// Unbind any actions or axes that are bound to the given chord
	void UnbindChord(FKey Key, bool ShiftDown = false, bool CtrlDown = false, bool AltDown = false, bool CmdDown = false);

	bool operator==(const FInputMappingGroup& Other) const
	{
		return (ActionMappings == Other.ActionMappings
			&& AxisMappings == Other.AxisMappings);
	}

};

// A complete set of input mappings making up a preset
USTRUCT(Blueprintable, BlueprintType)
struct FInputMappingPreset
{
	GENERATED_USTRUCT_BODY()
	
	// Tag for this preset
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Input Mapping Preset")
	FGameplayTag PresetTag;

	// If true, use default mappings defined in Input page in project settings
	UPROPERTY(config, EditAnywhere, Category = "Input Mapping Preset")
	bool bUseDefaultMappings;

	// Input mappings for this preset if not using default mappings
	UPROPERTY(config, EditAnywhere, Category = "Input Mapping Preset", meta = (EditCondition = "!bUseDefaultMappings"))
	TArray<FInputMappingGroup> MappingGroups;

	FInputMappingPreset()
		: bUseDefaultMappings(false)
	{}

	FInputMappingPreset(FGameplayTag InTag, bool InUseDefaultMappings)
	{
		PresetTag = InTag;
		bUseDefaultMappings = InUseDefaultMappings;
	}

	// Construct groups from a list of actions and axis, such as default UE input
	FInputMappingPreset(FGameplayTag InTag, TArray<FInputActionKeyMapping> ActionMappings, TArray<FInputAxisKeyMapping> AxisMappings);

	// Returns mapping groups that which should remove bindings that conflict with ones in the given group
	TArray<int32> GetMappingGroupsToUnbind(int32 SourceMappingGroup) const;

	// Set the mappings for this preset, which constructs Mapping Groups to store them in
	void SetMappings(TArray<FInputActionKeyMapping> ActionMappings, TArray<FInputAxisKeyMapping> AxisMappings);

	// Add the given action and remove any existing actions that it should replace
	void ReplaceAction(const FInputActionKeyMapping& Action, int32 InputGroupId, bool bAnyKeyGroup = false);

	// Add the given axis and remove any existing axes that it should replace
	void ReplaceAxis(const FInputAxisKeyMapping& Axis, int32 InputGroupId, bool bAnyKeyGroup = false);

	// Unbind any actions or axes that are bound to the given chord
	void UnbindChord(FKey Key, TArray<int32> MappingGroupIds, bool ShiftDown = false, bool CtrlDown = false, bool AltDown = false, bool CmdDown = false);

	// Return all action mappings in this preset
	TArray<FInputActionKeyMapping> GetActions() const
	{
		TArray<FInputActionKeyMapping> Actions;
		for (FInputMappingGroup Group : MappingGroups)
		{
			for (FInputActionKeyMapping& Action : Group.ActionMappings)
			{
				Actions.AddUnique(Action);
			}
		}
		return Actions;
	}

	// Return all axis mappings in this preset
	TArray<FInputAxisKeyMapping> GetAxes() const
	{
		TArray<FInputAxisKeyMapping> Axes;
		for (FInputMappingGroup Group : MappingGroups)
		{
			for (FInputAxisKeyMapping& Axis : Group.AxisMappings)
			{
				Axes.AddUnique(Axis);
			}
		}
		return Axes;
	}

	// Apply this preset to the given player controller
	void Apply(APlayerController* Player);

	bool operator==(const FInputMappingPreset& Other) const
	{
		return (PresetTag == Other.PresetTag
			&& bUseDefaultMappings == Other.bUseDefaultMappings
			&& MappingGroups == Other.MappingGroups);
	}

};

// Defines an icon texture for a key
USTRUCT()
struct FKeyIconPair
{
	GENERATED_BODY()

	// Key that the icon is for
	UPROPERTY(config, EditAnywhere, Category = "Key Icon Pair")
	FKey Key;

	// Icon texture to be assigned to the key
	UPROPERTY(EditAnywhere, Category = "Key Icon Pair")
	UTexture* Icon;
};

// Set of key icons
USTRUCT()
struct FKeyIconSet
{
	GENERATED_BODY()
	
	// Tags to identify this icon set
	// May include platform such as "XBox" or "PlayStation" and/or variations such as "Small" or "Large"
	UPROPERTY(config, EditAnywhere, Category = "Key Icon Set")
	FGameplayTagContainer Tags;

	// Icons defined for different keys
	UPROPERTY(config, EditAnywhere, Category = "Key Icon Set", meta = (TitleProperty = "Key"))
	TArray<FKeyIconPair> Icons;

	// Return the icon texture for the given key
	UTexture* GetIcon(FKey Key) const;
};

// Defines a group of keys which should be separated from other keys
USTRUCT()
struct FKeyGroup
{
	GENERATED_BODY()
	
	// Tag to identify this key group
	UPROPERTY(config, EditAnywhere, Category = "Key Group")
	FGameplayTag KeyGroupTag;

	// If true, will include all gamepad keys in this key group
	UPROPERTY(config, EditAnywhere, Category = "Key Group")
	bool bUseGamepadKeys;

	// If true, will include all keys that are not gamepad keys in this key group
	UPROPERTY(config, EditAnywhere, Category = "Key Group")
	bool bUseNonGamepadKeys;

	// Keys to include in this key group
	UPROPERTY(config, EditAnywhere, Category = "Key Group", meta = (TitleProperty = "KeyName"))
	TArray<FKey> Keys;

	bool Contains(FKey Key) const
	{
		const bool Gamepad = Key.IsGamepadKey();
		if (Gamepad && bUseGamepadKeys)
			return true;
		if (!Gamepad && bUseNonGamepadKeys)
			return true;
		return Keys.Contains(Key);
	}
};

// Represents pairing of a button key and a scale, used to associate buttons with an axis
USTRUCT()
struct FKeyScale
{
	GENERATED_BODY()

	// Button key
	UPROPERTY(config, EditAnywhere, Category = "Key Scale")
	FKey Key;

	// Scale of the axis that the button key should be associated with
	UPROPERTY(config, EditAnywhere, Category = "Key Scale")
	float Scale;

	FKeyScale()
		: Scale(1.f)
	{
	}

	FKeyScale(FKey InKey, float InScale)
	{
		Key = InKey;
		Scale = InScale;
	}

};

// Defines a set of mapping groups which are linked
USTRUCT()
struct FMappingGroupLink
{
	GENERATED_BODY()

	UPROPERTY(config, EditAnywhere, Category = "Mapping Group Link")
	TArray<int32> MappingGroups;

};

// Determines association between axis and buttons
// Used to tell the system that for example "Gamepad Left Stick Y" axis with a scale of -1 should be associated with "Gamepad Left Stick Down"
USTRUCT()
struct FAxisAssociation
{
	GENERATED_BODY()
	
	// Axis key for this association
	UPROPERTY(config, EditAnywhere, Category = "Axis Association")
	FKey AnalogKey;

	// Button keys to associate axis with
	UPROPERTY(config, EditAnywhere, Category = "Axis Association", meta = (TitleProperty = "Key"))
	TArray<FKeyScale> BooleanKeys;

};

// Defines friendly user-facing name text for a key
USTRUCT()
struct FKeyFriendlyName
{
	GENERATED_BODY()

	// Specified key
	UPROPERTY(config, EditAnywhere, Category = "Key Friendly Name")
	FKey Key;

	// User-facing friendly text name
	UPROPERTY(config, EditAnywhere, Category = "Key Friendly Name")
	FText FriendlyName;
};

// Plugin configuration page in Project Settings
UCLASS(config=Game, defaultconfig)
class AUTOSETTINGS_API UAutoSettingsConfig : public UObject
{
	GENERATED_BODY()

public:

	UAutoSettingsConfig();

	// Player Controllers need to be initialized with AutoSettings to set up their custom input overrides, otherwise they will still be using the default mappings
	// If true, the system will automatically try to initialize the correct input overrides for new PlayerControllers it finds, which is the easiest option
	// If false, input can be manually initialized per PlayerController with InitializePlayerInput for greater flexibility
	UPROPERTY(config, EditAnywhere, Category = Input)
	bool bAutoInitializePlayerInputOverrides;

	// Allow Shift, Ctrl, Alt, Cmd modifiers for action mappings
	// Note: Axis mappings cannot have modifiers
	UPROPERTY(config, EditAnywhere, Category = Input)
	bool AllowModifierKeys;

	// If not empty, text to use for shift modifier labels if an icon is not found
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (EditCondition = "AllowModifierKeys"))
	FText ShiftModifierOverrideText;

	// If not empty, text to use for ctrl modifier labels if an icon is not found
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (EditCondition = "AllowModifierKeys"))
	FText CtrlModifierOverrideText;

	// If not empty, text to use for alt modifier labels if an icon is not found
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (EditCondition = "AllowModifierKeys"))
	FText AltModifierOverrideText;

	// If not empty, text to use for cmd modifier labels if an icon is not found
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (EditCondition = "AllowModifierKeys"))
	FText CmdModifierOverrideText;

	// Input mapping presets for this project
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (TitleProperty = "PresetTag"))
	TArray<FInputMappingPreset> InputPresets;

	// If false, new bindings will remove key from all other mappings
	UPROPERTY(config, EditAnywhere, Category = Input)
	bool AllowMultipleBindingsPerKey;

	// (Advanced) If AllowMultipleBindingsPerKey is true, this determines mapping groups which should remain "linked" with unique bindings
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (EditCondition = "AllowMultipleBindingsPerKey", TitleProperty = "MappingGroups"))
	TArray<FMappingGroupLink> MappingGroupLinks;

	// Actions in this list will be ignored when saving/applying bindings
	// Eg. You have UI-related actions that should remain bound even when the same inputs are bound to in-game actions
	UPROPERTY(config, EditAnywhere, Category = Input)
	TArray<FName> BlacklistedActions;

	// Axes in this list will be ignored when saving/applying bindings
	// See BlacklistedActions
	UPROPERTY(config, EditAnywhere, Category = Input)
	TArray<FName> BlacklistedAxes;

	// Sets of key icons for this project
	// Example sets might include "Xbox Small" or "Playstation Transparent", where the platform is determined on a global application-wide level and the variant is determined by location in game
	UPROPERTY(config, EditAnywhere, Category = Input)
	TArray<FKeyIconSet> KeyIconSets;

	// Friendly key names to use if an icon is not available
	// If a friendly name is not specified for a key, the default FKey GetDisplayName will be used
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (TitleProperty = "Key"))
	TArray<FKeyFriendlyName> KeyFriendlyNames;

	// Groups of keys to separate from other keys
	// Useful if you want a separate column for gamepad controls in your input UI or other similar cases
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (TitleProperty = "KeyGroupTag"))
	TArray<FKeyGroup> KeyGroups;

	// Key whitelist, use to allow specific keys for input mapping
	// Leave empty to allow all keys
	// Keys here will have no effect if also in DisallowedKeys
	UPROPERTY(config, EditAnywhere, Category = Input)
	TArray<FKey> AllowedKeys;

	// Key blacklist, use to disallow specific keys for input mapping
	// Will take precedence over AllowedKeys
	UPROPERTY(config, EditAnywhere, Category = Input)
	TArray<FKey> DisallowedKeys;

	// If any of these keys are pressed while listening for input binding, it will instead cancel and keep the original binding
	UPROPERTY(config, EditAnywhere, Category = Input)
	TArray<FKey> BindingEscapeKeys;

	// Distance the mouse must travel before it will trigger a capture for a mouse axis while listening for input bindings
	// Lower values are more sensitive, higher values mean you have to move the mouse further
	UPROPERTY(config, EditAnywhere, Category = Input)
	float MouseMoveCaptureDistance;

	// Determines association between axes and buttons
	// This is used during axis rebinding to determine which axis should be used when a key press is detected
	// e.g. If the system detects the player has pressed "Gamepad Left Stick Down" key when binding "Move Backward",
	// these associations could specify that the "Gamepad Left Stick Y" axis should be bound with a scale of -1
	// otherwise the mapping will only have 2 key states (pressed and not pressed) rather than the full analog range of the axis
	UPROPERTY(config, EditAnywhere, Category = Input, meta = (TitleProperty = "AnalogKey"))
	TArray<FAxisAssociation> AxisAssociations;

	// Returns all input presets in config or construct one from Input settings if none are found
	TArray<FInputMappingPreset> GetInputPresets() const;

	// Returns the icon texture for the given key and key icon tags
	UTexture* GetIconForKey(FKey InKey, FGameplayTagContainer Tags) const;

	// Returns the Friendly Name override for the key if available (specified in AutoSettings config) or falls back to the FKey DisplayName
	FText GetKeyFriendlyName(FKey Key) const;

	// Returns the key group that contains the given key
	UFUNCTION(BlueprintPure, Category = "Auto Settings Config", DisplayName = "Get Key Friendly Name")
	static FText GetKeyFriendlyNameStatic(FKey Key);

	// Returns the key group that contains the given key
	UFUNCTION(BlueprintPure, Category = "Auto Settings Config", DisplayName = "Get Key Group")
	static FGameplayTag GetKeyGroupStatic(FKey Key);

	// Returns true if a Key Group with the specified tag exists and contains the given key
	bool DoesKeyGroupContainKey(FGameplayTag KeyGroupTag, FKey Key) const;

	// True if there is actually a key group definition with the given tag
	bool IsKeyGroupDefined(FGameplayTag KeyGroupTag) const;

	FGameplayTag GetKeyGroup(FKey Key) const;

	// True if both keys are in the same key group
	bool SameKeyGroup(FKey KeyA, FKey KeyB) const;

	// Returns analog key (Axis) for the given boolean key (Button)
	FKeyScale GetAnalogKey(FKey InBooleanKey) const;

	// True if the given key is an analog key/axis in any stored Axis Association
	bool IsAnalogKey(FKey Key) const;

	// True if the given key is whitelisted (if applicable) and not blacklisted
	bool IsKeyAllowed(FKey Key) const;

	// True if bindings should be kept unique between two mapping groups
	bool ShouldBindingsBeUniqueBetweenMappingGroups(int32 MappingGroupA, int32 MappingGroupB) const;

};