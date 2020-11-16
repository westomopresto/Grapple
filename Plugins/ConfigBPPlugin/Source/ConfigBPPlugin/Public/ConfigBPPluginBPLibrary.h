// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#pragma once

#include "Misc/ConfigCacheIni.h"
#include "Engine.h"
#include "FileOpenState.h"
#include "ConfigBPPluginBPLibrary.generated.h"

class UConfigFile;

/* 
*	ConfigBP Function library class.
*	
*	Main class for ConfigBP - This contains most to all of the functionality that ConfigBP exposes to blueprints.
*	Includes functionality for getting .ini files, opening them, and reading/writing data from/to the files.
*/
UCLASS()
class UConfigBPPluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	// Returns the location of the Game.ini file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Game.ini Location", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FString GetGameIniLocation(bool absolute);

	// Returns the location of the Engine.ini file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Engine.ini Location", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FString GetEngineIniLocation(bool absolute);
	
	// Returns the location of the Scalability.ini file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Scalability.ini Location", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FString GetScalabilityIniLocation(bool absolute);

	// Returns the location of the Input.ini file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Input.ini Location", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FString GetInputIniLocation(bool absolute);

	// Returns the location of the GameUserSettings.ini file
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get GameUserSettings.ini Location", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FString GetGameUserSettingsIniLocation(bool absolute);

	/*
	 * Opens up a configuration file. Accepts relative and absolute paths.
	 *
	 * @param Path The (absolute or relative) path of the configuration file you're trying to use; can contain parsers.
	 * @param isAbsolute Whether the path is absolute (i.e. a full path, such as C:/MyGame/test.ini) or relative (i.e. test.ini)
	 *
	 * @param permissionLevel What level of access ConfigBP could get to that file
	 * @param bDidFileExist True when the file existed before the node was called, false when it was created by ConfigBP.
	 * @paramn bSuccess Whether or not the configuration file was successfully opened.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open configuration file", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static UConfigFile *OpenFile(FString Path, bool isAbsolute, EFileOpenState &permissionLevel, bool &bDidFileExist, bool &bSuccess);


	// Get a key from the configuration file as a 32 bit integer
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As An Integer", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static int32 GetParameterAsInt(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as a float
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Float", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static float GetParameterAsFloat(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as a string
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A String", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FString GetParameterAsString(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as a boolean
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Boolean", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static bool GetParameterAsBool(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as a double
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Double", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static float GetParameterAsDouble(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as an FColor
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Color", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FColor GetParameterAsColor(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as an FRotator
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Rotator", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FRotator GetParameterAsRotator(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as FText
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As Text", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FText GetParameterAsText(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as an FVector
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Vector", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FVector GetParameterAsVector3(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as an FVector2D
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A 2D Vector", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FVector2D GetParameterAsVector2(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as an FTransform
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A Transform", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static FTransform GetParameterAsTransform(UConfigFile *File, FString Section, FString Key, bool &success);

	// Get a key from the configuration file as a string array
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Parameter As A String Array", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static TArray<FString> GetParameterAsStringArray(UConfigFile *File, FString Section, FString Key, bool &success);


	// Store key to configuration file as a 32 bit integer
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Integer Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetIntParameter(UConfigFile *File, int32 Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as a float
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Float Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetFloatParameter(UConfigFile *File, float Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as a string
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set String Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetStringParameter(UConfigFile *File, FString Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as a boolean
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Boolean Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetBoolParameter(UConfigFile *File, bool Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as a double
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Double Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetDoubleParameter(UConfigFile *File, float Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as an FColor
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Color Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetColorParameter(UConfigFile *File, FColor Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as an FRotator
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Rotator Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetRotatorParameter(UConfigFile *File, FRotator Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as text
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Text Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetTextParameter(UConfigFile *File, FText Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as an FVector
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Vector Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetVector3Parameter(UConfigFile *File, FVector Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as an FVector2D
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set 2D Vector Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetVector2Parameter(UConfigFile *File, FVector2D Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as an FTransform
	/// Get's saved as key_R (rotation), key_T (translation) and key_S (scale)
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Transform Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetTransformParameter(UConfigFile *File, FTransform Value, FString Section, FString Key, bool &success);

	// Store key to configuration file as a string array
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set String Array Parameter", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static void SetStringArrayParameter(UConfigFile *File, TArray<FString> Value, FString Section, FString Key, bool &success);

	/* NOTE: This functionality will be shipping with a future update
	// Force the rebuilding of User Input InputMaps. NOTE: Be careful when manually updating the input configuration files!
	// WARNING: This function is experimental and should not be used unless absolutely required. We're still working on refining it,and cannot guarantee that this will work.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Rebind Input Configuration/Maps [EXPERIMENTAL]", Keywords = "ConfigBPPlugin Rebind Input"), Category = "ConfigBP")
	static void RebindInputMaps();
	*/

	// (Forcefully) reload a configuration file. NOTE: When opening a configuration file it is automatically reloaded.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reload Configuration File", Keywords = "ConfigBPPlugin Reload reloadconfig"), Category = "ConfigBP")
	static void ReloadConfig(UConfigFile *File);

	/**
	 * Returns all of the section names in a given configuration file
	 * 
	 * @param File The configuration file you'd like to retrieve the sections for
	 * @param bSuccess Whether or not the sections could be obtained
	 * 
	 * @return The array of section names
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Configuration File Sections", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static TArray<FString> GetSections(UConfigFile *File, bool &bSuccess);

	/**
	 * Returns all of the keys in a given configuration file's section
	 *
	 * @param File The configuration file the section is in
	 * @param Section The section you'd like to retrieve the keys for
	 *
	 * @return The array of keys
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Section Keys", Keywords = "ConfigBPPlugin"), Category = "ConfigBP")
	static TArray<FString> GetSectionKeys(UConfigFile *File, FString Section, bool &bSuccess);
};
