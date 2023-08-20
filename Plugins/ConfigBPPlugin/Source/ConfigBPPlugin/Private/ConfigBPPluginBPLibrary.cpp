// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#include "ConfigBPPluginBPLibrary.h"
#include "ConfigBPPlugin.h"
#include "ConfigFile.h"

UConfigBPPluginBPLibrary::UConfigBPPluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UConfigBPPluginBPLibrary::GetGameIniLocation(bool absolute)
{
	// Ensure GConfigis valid (as it's in charge of setting the GGameIni variable
	if (!GConfig)
		return FString();

	if (absolute)
		return GGameIni;

	// Get the path of GGameIni relative to the Generated Config Directory (Saved/Config)
	FString GameIniPath = GGameIni;
	FPaths::MakePathRelativeTo(GameIniPath, *FPaths::GeneratedConfigDir());

	// TODO use FPaths::GetPath if people would prefer Saved/Config/SamePlatform instead

	return GameIniPath;
}

FString UConfigBPPluginBPLibrary::GetEngineIniLocation(bool absolute)
{
	// Ensure GConfig is valid (as it's in charge of setting the GEngineIni variable
	if (!GConfig)
		return FString();

	if (absolute)
		return GEngineIni;
	
	// Get the path of GEngineIni relative to the Generated Config Directory (Saved/Config)
	FString EngineIniPath = GEngineIni;
	FPaths::MakePathRelativeTo(EngineIniPath, *FPaths::GeneratedConfigDir());

	// TODO use FPaths::GetPath if people would prefer Saved/Config/SamePlatform instead

	return EngineIniPath;
}

FString UConfigBPPluginBPLibrary::GetScalabilityIniLocation(bool absolute)
{
	// Ensure GConfig is valid (as it's in charge of setting the GScalabilityIni variable
	if (!GConfig)
		return FString();

	if (absolute)
		return GScalabilityIni;
	
	// Get the path of GScalabilityIni relative to the Generated Config Directory (Saved/Config)
	FString ScalabilityIniPath = GScalabilityIni;
	FPaths::MakePathRelativeTo(ScalabilityIniPath, *FPaths::GeneratedConfigDir());

	// TODO use FPaths::GetPath if people would prefer Saved/Config/SamePlatform instead

	return ScalabilityIniPath;
}

FString UConfigBPPluginBPLibrary::GetInputIniLocation(bool absolute)
{
	// Ensure GConfig is valid (as it's in charge of setting the GInputIni variable
	if (!GConfig)
		return FString();

	if (absolute)
		return GInputIni;
	
	// Get the path of GInputIni relative to the Generated Config Directory (Saved/Config)
	FString InputIniPath = GInputIni;
	FPaths::MakePathRelativeTo(InputIniPath, *FPaths::GeneratedConfigDir());

	// TODO use FPaths::GetPath if people would prefer Saved/Config/SamePlatform instead

	return InputIniPath;
}

FString UConfigBPPluginBPLibrary::GetGameUserSettingsIniLocation(bool absolute)
{
	// Ensure GConfig is valid (as it's in charge of setting the GGameUserSettingsIni variable
	if (!GConfig)
		return FString();

	if (absolute)
		return GGameUserSettingsIni;
	
	// Get the path of GGameUserSettingsIni relative to the Generated Config Directory (Saved/Config)
	FString GameUserSettingsIniPath = GGameUserSettingsIni;
	FPaths::MakePathRelativeTo(GameUserSettingsIniPath, *FPaths::GeneratedConfigDir());

	// TODO use FPaths::GetPath if people would prefer Saved/Config/SamePlatform instead

	return GameUserSettingsIniPath;
}

UConfigFile *UConfigBPPluginBPLibrary::OpenFile(FString Path, bool isAbsolute, EFileOpenState &permissionLevel, bool &bDidFileExist, bool &bSuccess)
{
	FString ParsedPath = Path;
	// Parse the inputted path and replace various directory placeholders. NOTE: We can only do this if the path if note absolute:
	if (!isAbsolute)
	{
		ParsedPath.ReplaceInline(TEXT("{{UserDir}}"), *FPaths::ProjectUserDir()); // NOTE: This will give a game-specific AppData directory on Windows.
		ParsedPath.ReplaceInline(TEXT("{{GameConfig}}"), *FPaths::ProjectConfigDir());
		ParsedPath.ReplaceInline(TEXT("{{ProjectDir}}"), *FPaths::ProjectDir());
		ParsedPath.ReplaceInline(TEXT("{{ContentDir}}"), *FPaths::ProjectContentDir());
		ParsedPath.ReplaceInline(TEXT("{{ProjectSavedDir}}"), *FPaths::ProjectSavedDir());

		// TODO: More parsers will be implemented in the future!
	}


	// Should it convert the relative path to an absolute path? (and ensure we didn't use any of the parsers which auto to making it absolute)
	if (!isAbsolute && ParsedPath == Path)
	{
		// Convert the path into a full path to the Config directory and go into the directory/file specified by the Path variable
		Path = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::GeneratedConfigDir(), Path));
	}
	// ParsedPath will be different or the same, but we can override the variable value anyway.
	else
	{
		Path = FPaths::ConvertRelativePathToFull(ParsedPath);
	}

	// Create a platform file manager
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Does the file exist?
	if (!PlatformFile.FileExists(*Path))
	{
		UE_LOG(LogTemp, Warning, TEXT("Configuration doesn't exist at %s, so creating a blank file!"), *Path);
		bDidFileExist = false;

		// File doesn't exist; (Attempt to) create a new one.
		if (!FFileHelper::SaveStringToFile(TEXT(""), *Path))
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to create blank configuration file at %s!"), *Path);
			// We were unable to create the file, so therefore fail/return an "unsuccessful" error.
			bSuccess = false;
			permissionLevel = EFileOpenState::FOS_Error;
			
			// Other functions check the validity, so we can "safely" return a nullptr here.
			return nullptr;
		}
	}
	else
	{
		// File already exists/was not newly created.
		bDidFileExist = true;
	}

	// Which permission level do we have to the file?
	if (PlatformFile.IsReadOnly(*Path))
		// Only read access
		permissionLevel = EFileOpenState::FOS_ReadOnly;
	else
		// Read and write access
		permissionLevel = EFileOpenState::FOS_ReadWrite;

	// Pre-/Reload the configuration file.
	// NOTE: This line is no longer being used due to potential crashes on the second call when loading engine files inside of the editor.
	//GConfig->LoadFile(Path);

	// Create a config file object
	UConfigFile *configFile = NewObject<UConfigFile>();
	configFile->InitializeObject(Path, permissionLevel);

	UE_LOG(LogTemp, Warning, TEXT("Successfully opened configuration file at %s"), *Path);

	// Everything completed successfully!
	bSuccess = true;

	return configFile;
}

int32 UConfigBPPluginBPLibrary::GetParameterAsInt(UConfigFile *File, FString Section, FString Key, bool &success)
{
	int32 value = 0;
	if (!IsValid(File) || !GConfig || !GConfig->GetInt(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		return 0;
	}
	else
	{
		success = true;
		return value;
	}
}

float UConfigBPPluginBPLibrary::GetParameterAsFloat(UConfigFile *File, FString Section, FString Key, bool & success)
{
	float value = 0.f;
	if (!IsValid(File) || !GConfig || !GConfig->GetFloat(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		return 0.f;
	}
	else
	{
		success = true;
		return value;
	}
}

FString UConfigBPPluginBPLibrary::GetParameterAsString(UConfigFile *File, FString Section, FString Key, bool & success)
{
	FString value = FString();
	if (!IsValid(File) || !GConfig || !GConfig->GetString(*Section, *Key, value, File->GetFilePath()))
	{
		if (!IsValid(File))
		{
			UE_LOG(LogTemp, Error, TEXT("Configuration file read failed with reason: invalid configuration file."))
		}
		if (!GConfig)
		{
			UE_LOG(LogTemp, Error, TEXT("Configuration file read failed with reason: unable to use GConfig."))
		}
		if (!GConfig->GetString(*Section, *Key, value, File->GetFilePath()))
		{
			UE_LOG(LogTemp, Error, TEXT("Configuration file read failed with reason: unable to read value; section likely does not exist. (Section: %s; Key: %s; File Path: %s"), *Section, *Key, *File->GetFilePath())
		}

		success = false;
		return FString();
	}
	else
	{
		success = true;
		return value;
	}
}

bool UConfigBPPluginBPLibrary::GetParameterAsBool(UConfigFile *File, FString Section, FString Key, bool & success)
{
	bool value = false;
	if (!IsValid(File) || !GConfig || !GConfig->GetBool(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		return false;
	}
	else
	{
		success = true;
		return value;
	}
}

float UConfigBPPluginBPLibrary::GetParameterAsDouble(UConfigFile *File, FString Section, FString Key, bool & success)
{
	double value = 0.f;
	if (!IsValid(File) || !GConfig || !GConfig->GetDouble(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		return 0.f;
	}
	else
	{
		success = true;
		// Typecast into float as blueprint doesn't support doubles
		return (float)value;
	}
}

FColor UConfigBPPluginBPLibrary::GetParameterAsColor(UConfigFile *File, FString Section, FString Key, bool & success)
{
	FColor value = FColor(0, 0, 0);
	if (!IsValid(File) || !GConfig || !GConfig->GetColor(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		// Return black
		return FColor(0, 0, 0);
	}
	else
	{
		success = true;
		return value;
	}
}

FRotator UConfigBPPluginBPLibrary::GetParameterAsRotator(UConfigFile *File, FString Section, FString Key, bool & success)
{
	FRotator value = FRotator(0.f, 0.f, 0.f);
	if (!IsValid(File) || !GConfig || !GConfig->GetRotator(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		// Return (0, 0, 0) rotation
		return FRotator(0.f, 0.f, 0.f);
	}
	else
	{
		success = true;
		return value;
	}
}

FText UConfigBPPluginBPLibrary::GetParameterAsText(UConfigFile *File, FString Section, FString Key, bool & success)
{
	FText value = FText();
	if (!IsValid(File) || !GConfig || !GConfig->GetText(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		// Return blank text
		return FText();
	}
	else
	{
		success = true;
		return value;
	}
}

FVector UConfigBPPluginBPLibrary::GetParameterAsVector3(UConfigFile *File, FString Section, FString Key, bool & success)
{
	FVector value = FVector(0.f, 0.f, 0.f);
	if (!IsValid(File) || !GConfig || !GConfig->GetVector(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		// Return (0, 0, 0) vector
		return FVector(0.f, 0.f, 0.f);
	}
	else
	{
		success = true;
		return value;
	}
}

FVector2D UConfigBPPluginBPLibrary::GetParameterAsVector2(UConfigFile *File, FString Section, FString Key, bool & success)
{
	FVector2D value = FVector2D(0.f, 0.f);
	if (!IsValid(File) || !GConfig || !GConfig->GetVector2D(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		// Return (0, 0) vector
		return FVector2D(0.f, 0.f);
	}
	else
	{
		success = true;
		return value;
	}
}

FTransform UConfigBPPluginBPLibrary::GetParameterAsTransform(UConfigFile *File, FString Section, FString Key, bool &success)
{
	/*
	* Key_L -> Location
	* Key_R -> Rotation
	* Key_S -> Scale
	*/

	FString locationKey = Key + TEXT("_L");
	FString rotationKey = Key + TEXT("_R");
	FString scaleKey = Key + TEXT("_S");

	bool wasSuccessful = false;

	FVector location = GetParameterAsVector3(File, Section, locationKey, OUT wasSuccessful);
	if (!wasSuccessful)
		location = FVector(0.f, 0.f, 0.f);

	FRotator rotation = GetParameterAsRotator(File, Section, rotationKey, OUT wasSuccessful);
	if (!wasSuccessful)
		rotation = FRotator(0.f, 0.f, 0.f);

	FVector scale = GetParameterAsVector3(File, Section, scaleKey, OUT wasSuccessful);
	if (!wasSuccessful)
		scale = FVector(0.f, 0.f, 0.f);

	return FTransform(rotation, location, scale);
}

TArray<FString> UConfigBPPluginBPLibrary::GetParameterAsStringArray(UConfigFile *File, FString Section, FString Key, bool &success)
{
	TArray<FString> value;
	if (!IsValid(File) || !GConfig || !GConfig->GetArray(*Section, *Key, value, File->GetFilePath()))
	{
		success = false;
		return TArray<FString>();
	}
	else
	{
		success = true;
		return value;
	}
}

void UConfigBPPluginBPLibrary::SetIntParameter(UConfigFile *File, int32 Value, FString Section, FString Key, bool &success)
{
	if(!IsValid(File) && !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetInt(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetFloatParameter(UConfigFile *File, float Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetFloat(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetStringParameter(UConfigFile *File, FString Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetString(*Section, *Key, *Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetBoolParameter(UConfigFile *File, bool Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetBool(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetDoubleParameter(UConfigFile * File, float Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetDouble(*Section, *Key, (double)Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetColorParameter(UConfigFile * File, FColor Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetColor(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetRotatorParameter(UConfigFile * File, FRotator Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetRotator(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetTextParameter(UConfigFile * File, FText Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetText(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetVector3Parameter(UConfigFile * File, FVector Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetVector(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetVector2Parameter(UConfigFile * File, FVector2D Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetVector2D(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

void UConfigBPPluginBPLibrary::SetTransformParameter(UConfigFile * File, FTransform Value, FString Section, FString Key, bool & success)
{
	/*
	* Key_L -> Location
	* Key_R -> Rotation
	* Key_S -> Scale
	*/

	FString locationKey = Key + TEXT("_L");
	FString rotationKey = Key + TEXT("_R");
	FString scaleKey = Key + TEXT("_S");

	bool wasSuccessful = false;


	SetVector3Parameter(File, Value.GetTranslation(), Section, locationKey, OUT wasSuccessful);

	if (!wasSuccessful)
	{
		success = false;
		return;
	}

	SetRotatorParameter(File, Value.Rotator(), Section, rotationKey, OUT wasSuccessful);
	if (!wasSuccessful)
	{
		success = false;
		return;
	}
	
	SetVector3Parameter(File, Value.GetScale3D(), Section, scaleKey, OUT wasSuccessful);
	if (!wasSuccessful)
	{
		success = false;
		return;
	}

	success = true;
}

void UConfigBPPluginBPLibrary::SetStringArrayParameter(UConfigFile * File, TArray<FString> Value, FString Section, FString Key, bool & success)
{
	if (!IsValid(File) || !GConfig)
	{
		success = false;
		return;
	}

	GConfig->SetArray(*Section, *Key, Value, File->GetFilePath());
	GConfig->Flush(false, File->GetFilePath());

	success = true;
}

/*
void UConfigBPPluginBPLibrary::RebindInputMaps()
{
	// Traverse over the player input objects
	for (TObjectIterator<UPlayerInput> Iter; Iter; ++Iter)
	{
		Iter->ForceRebuildingKeyMaps();
	}
}
*/

void UConfigBPPluginBPLibrary::ReloadConfig(UConfigFile *File)
{
	GConfig->LoadFile(File->GetFilePath());
}

TArray<FString> UConfigBPPluginBPLibrary::GetSections(UConfigFile *File, bool &bSuccess)
{
	if (!IsValid(File) || !GConfig)
	{
		bSuccess = false;
		return TArray<FString>();
	}

	TArray<FString> SectionNames;
	bSuccess = GConfig->GetSectionNames(File->GetFilePath(), SectionNames);
	return SectionNames;
}

TArray<FString> UConfigBPPluginBPLibrary::GetSectionKeys(UConfigFile *File, FString Section, bool &bSuccess)
{
	if (!IsValid(File) || !GConfig || Section.IsEmpty())
	{
		bSuccess = false;
		return TArray<FString>();
	}

	// Attempt to get the section's lines (there isn't a way to get the keys directly from what I can tell), and extract the keys assuming they are what comes left of the equals sign
	TArray<FString> SectionLines;
	bSuccess = GConfig->GetSection(*Section, SectionLines, File->GetFilePath());

	TArray<FString> Keys;
	for (auto &Line : SectionLines)
	{
		FString LeftStr;
		FString RightStr;
		if (!Line.Split(TEXT("="), &LeftStr, &RightStr) || LeftStr.IsEmpty())
		{
			// Unable to obtain the key; skip
			UE_LOG(LogTemp, Warning, TEXT("Unable to parse key from \"%s\"; skipping..."), *Line);
			continue;
		}
		else
		{
			// Successfully parsed key
			Keys.Add(LeftStr);
		}
	}

	return Keys;
}
