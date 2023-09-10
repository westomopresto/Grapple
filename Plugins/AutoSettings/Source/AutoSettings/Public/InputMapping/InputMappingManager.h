// Copyright Sam Bonifacio. All Rights Reserved.

#pragma once

#include "Misc/AutoSettingsConfig.h"
#include "Tickable.h"
#include "InputMappingManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAutoSettingsInput, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMappingsChangedEvent, APlayerController*, Player);

// Holds saved input mappings for a specific player, as well as if they are custom or from a preset
// Used to save and load input mapping information
USTRUCT(BlueprintType)
struct FPlayerInputMappings
{
	GENERATED_USTRUCT_BODY()
	
	// String to identify the player that owns these mappings
	// Preferably the Unique Net ID of the player, but can fall back to a Controller ID
	UPROPERTY()
	FString PlayerId;
	
	// Deprecated, PlayerId should be used instead
	// Only exists for backwards compatibility
	UPROPERTY()
	int32 PlayerIndex;

	// True if the mapping holds player-specific overrides, false if using an unmodified preset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mapping")
	bool Custom;

	// Mapping preset which may or may not have been modified by player-specific overrides
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mapping")
	FInputMappingPreset Preset;

	// Key group used to retrieve active mappings
	// Useful for globally switching prompts between key groups when for example input device is changed
	// See UInputMappingManager::SetPlayerKeyGroupStatic
	UPROPERTY()
	FGameplayTag PlayerKeyGroup;

	FPlayerInputMappings() : PlayerIndex(-1), Custom(false)
	{}

	FPlayerInputMappings(FString PlayerId, bool Custom, FInputMappingPreset Preset)
		: PlayerId(PlayerId),
		PlayerIndex(-1),
		Custom(Custom),
		Preset(Preset)
	{
	}

	bool operator==(const FPlayerInputMappings& Other) const
	{
		return (PlayerId == Other.PlayerId
			&& Custom == Other.Custom
			&& Preset == Other.Preset
			&& PlayerKeyGroup == Other.PlayerKeyGroup);
	}

	bool operator!=(const FPlayerInputMappings& Other) const
	{
		return !(*this == Other);
	}
};

/**
 * Manages input mapping for players
 */
UCLASS(Config = Input)
class AUTOSETTINGS_API UInputMappingManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	UInputMappingManager();

	// Fired when a player's input mappings are updated
	FMappingsChangedEvent OnMappingsChanged;

	// Get singleton instance
	static UInputMappingManager* Get();

	// Get input presets defined in project settings (AutoSettings page)
	UFUNCTION(BlueprintPure, Category = "Input Mapping")
	static TArray<FInputMappingPreset> GetDefaultInputPresets();

	// Get input mappings for player
	// Static version
	UFUNCTION(BlueprintPure, Category = "Input Mapping", DisplayName = "Get Player Input Mappings")
	static FPlayerInputMappings GetPlayerInputMappingsStatic(APlayerController* Player);

	// Set a player's input mapping preset
	// Static version
	UFUNCTION(BlueprintCallable, Category = "Input Mapping", DisplayName = "Set Player Input Preset")
	static void SetPlayerInputPresetStatic(APlayerController* Player, FInputMappingPreset Preset, bool bIsCustomized = false);

	// Set a player's input mapping preset by tag
	// Presets and tags are defined in project settings (AutoSettings page)
	// Static version
	UFUNCTION(BlueprintCallable, Category = "Input Mapping")
	static void SetPlayerInputPresetByTag(APlayerController* Player, FGameplayTag PresetTag);

	// Set key group used to retrieve active mappings for a player
	// Useful for globally switching prompts between key groups when for example input device is changed
	UFUNCTION(BlueprintCallable, Category = "Input Mapping", DisplayName = "Set Player Input Key Group")
	static void SetPlayerKeyGroupStatic(APlayerController* Player, FGameplayTag KeyGroup);

	// Override a player's action mapping on the given mapping group and save to config
	// @param bAnyKeyGroup If true will replace existing mappings on any key group instead of just the key group of the mapping specified.
	UFUNCTION(BlueprintCallable, Category = "Input Mapping", DisplayName = "Add Player Input Action Override")
	static void AddPlayerActionOverrideStatic(APlayerController* Player, const FInputActionKeyMapping& NewMapping, int32 MappingGroup, bool bAnyKeyGroup = false);

	// Override a player's axis mapping on the given mapping group
	// @param bAnyKeyGroup If true will replace existing mappings on any key group instead of just the key group of the mapping specified.
	UFUNCTION(BlueprintCallable, Category = "Input Mapping", DisplayName = "Add Player Input Axis Override")
	static void AddPlayerAxisOverrideStatic(APlayerController* Player, const FInputAxisKeyMapping& NewMapping, int32 MappingGroup, bool bAnyKeyGroup = false);

	// Initialize a player's input overrides manually
	UFUNCTION(BlueprintCallable, Category = "Input Mapping", DisplayName = "Initialize Player Input Overrides")
	static bool InitializePlayerInputOverridesStatic(APlayerController* Player);

	// Returns the mapping used for the given action
	// @param MappingGroup Mapping group index - if an input has multiple mappings, this will determine which one to use. A value of -1 will use the first mapping group available.
	UFUNCTION(BlueprintPure, Category = "Input Mapping", Displayname = "Get Player Action Mapping")
	static FInputActionKeyMapping GetPlayerActionMappingStatic(APlayerController* Player, FName ActionName, int32 MappingGroup = -1);

	// Returns the mapping used for the given axis and scale
	// @param MappingGroup Mapping group index - if an input has multiple mappings, this will determine which one to use. A value of -1 will use the first mapping group available.
	UFUNCTION(BlueprintPure, Category = "Input Mapping", Displayname = "Get Player Axis Mapping")
	static FInputAxisKeyMapping GetPlayerAxisMappingStatic(APlayerController* Player, FName AxisName, float Scale, int32 MappingGroup = -1);

	// Dumps registered player controllers to log with their input mappings
	static void DumpPlayers();

	void SetPlayerKeyGroup(APlayerController* Player, FGameplayTag KeyGroup);

	// Override a player's action mapping on the given mapping group and save to config
	// @param	bAnyKeyGroup	If true will replace existing mappings on any key group instead of just the key group of the mapping specified.
	UFUNCTION(BlueprintCallable, Category = "Input Mapping")
	void AddPlayerActionOverride(APlayerController* Player, const FInputActionKeyMapping& NewMapping, int32 MappingGroup, bool bAnyKeyGroup = false);

	// Override a player's axis mapping on the given mapping group
	// @param	bAnyKeyGroup	If true will replace existing mappings on any key group instead of just the key group of the mapping specified.
	UFUNCTION(BlueprintCallable, Category = "Input Mapping")
	void AddPlayerAxisOverride(APlayerController* Player, const FInputAxisKeyMapping& NewMapping, int32 MappingGroup, bool bAnyKeyGroup = false);

	// Get a player's action mapping on the given mapping group and key group
	FInputActionKeyMapping GetPlayerActionMapping(APlayerController* Player, FName ActionName, int32 MappingGroup, FGameplayTag KeyGroup, bool bUsePlayerKeyGroup = true ) const;

	// Get a player's axis mapping on the given mapping group and key group
	FInputAxisKeyMapping GetPlayerAxisMapping(APlayerController* Player, FName AxisName, float Scale, int32 MappingGroup, FGameplayTag KeyGroup, bool bUsePlayerKeyGroup = true) const;

	// Set a player's input mapping preset
	void SetPlayerInputPreset(APlayerController* Player, FInputMappingPreset Preset, bool bIsCustomized = false);

	// Set a player's input mapping preset by tag
	// Presets and tags are defined in project settings (AutoSettings page)
	void SetPlayerInputPreset(APlayerController* Player, FGameplayTag PresetTag);

protected:
	// FTickableGameObject
	void Tick(float DeltaTime) override;

	// Don't tick CDO
	bool IsTickable() const override { return !this->HasAnyFlags(RF_ClassDefaultObject); }

	TStatId GetStatId() const override { return TStatId(); }

private:
	static UInputMappingManager* Singleton;

	UPROPERTY(Config)
	TArray<FPlayerInputMappings> PlayerInputOverrides;

	UPROPERTY()
	TArray<APlayerController*> RegisteredPlayerControllers;

	UWorld* GetGameWorld() const;

	void RegisterPlayerController(APlayerController* Player);

	FPlayerInputMappings FindPlayerInputMappings(APlayerController* Player) const;

	void SavePlayerInputMappings(APlayerController* Player, FPlayerInputMappings& Mappings);

	UFUNCTION()
	void OnRegisteredPlayerControllerDestroyed(AActor* DestroyedActor);	
};
