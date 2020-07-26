// Copyright Sam Bonifacio. All Rights Reserved.

#include "SettingsManager.h"
#include "Logging/MessageLog.h"
#include "Scalability.h"
#include "Console/ConsoleUtils.h"
#include "Misc/ConfigCacheIni.h"
#include "AutoSettings.h"
#include "Utility/AutoSettingsStringUtils.h"

USettingsManager* USettingsManager::Singleton = nullptr;

USettingsManager* USettingsManager::Get()
{
	if (!Singleton)
	{
		Singleton = NewObject<USettingsManager>();
		Singleton->AddToRoot();
	}

	return Singleton;
}

FString USettingsManager::GetValue(FName Key, bool bPreferConfigValue)
{
	FString Value = bPreferConfigValue ? Get()->GetConfigValue(Key) : UConsoleUtils::GetStringCVar(Key);

	if (Value.IsEmpty())
	{
		// Fall back if preferred method is not available
		Value = bPreferConfigValue ? UConsoleUtils::GetStringCVar(Key) : Get()->GetConfigValue(Key);
	}

	return Value;
}

FString USettingsManager::GetInitialValue(FName Key)
{
	return GetValue(Key, false);
}

void USettingsManager::RegisterIntCVarSetting(FName Name, int32 DefaultValue, const FString& Help)
{
	if (UConsoleUtils::IsCVarRegistered(Name))
		return;

	const FString ConfigValue = Get()->GetConfigValue(Name);
	const int32 InitValue = ConfigValue.IsEmpty() ? DefaultValue : FCString::Atoi(*ConfigValue);

	UConsoleUtils::RegisterIntCVar(Name, InitValue, Help);	
}

void USettingsManager::RegisterBoolCVarSetting(FName Name, bool DefaultValue, const FString& Help)
{
	if (UConsoleUtils::IsCVarRegistered(Name))
		return;

	const FString ConfigValue = Get()->GetConfigValue(Name);
	const bool InitValue = ConfigValue.IsEmpty() ? DefaultValue : FAutoSettingsStringUtils::IsTruthy(ConfigValue); // Also accept int too

	UConsoleUtils::RegisterBoolCVar(Name, InitValue, Help);
}

void USettingsManager::RegisterFloatCVarSetting(FName Name, float DefaultValue, const FString& Help)
{
	if (UConsoleUtils::IsCVarRegistered(Name))
		return;

	const FString ConfigValue = Get()->GetConfigValue(Name);
	const float InitValue = ConfigValue.IsEmpty() ? DefaultValue : FCString::Atof(*ConfigValue);

	UConsoleUtils::RegisterFloatCVar(Name, InitValue, Help);
}

void USettingsManager::RegisterStringCVarSetting(FName Name, const FString& DefaultValue, const FString& Help)
{
	if (UConsoleUtils::IsCVarRegistered(Name))
		return;

	const FString ConfigValue = Get()->GetConfigValue(Name);
	const FString InitValue = ConfigValue.IsEmpty() ? DefaultValue : ConfigValue;

	UConsoleUtils::RegisterStringCVar(Name, InitValue, Help);
}

void USettingsManager::RegisterIntCVarSettingWithCallback(FName Name, int32 DefaultValue, const FString& Help, FIntCVarChangedSignature ChangedCallback, bool CallbackImmediately)
{
	RegisterIntCVarSetting(Name, DefaultValue, Help);
	UCVarChangeListenerManager::AddIntCVarCallbackStatic(Name, ChangedCallback, CallbackImmediately);
}

void USettingsManager::RegisterBoolCVarSettingWithCallback(FName Name, bool DefaultValue, const FString& Help, FBoolCVarChangedSignature ChangedCallback, bool CallbackImmediately)
{
	RegisterBoolCVarSetting(Name, DefaultValue, Help);
	UCVarChangeListenerManager::AddBoolCVarCallbackStatic(Name, ChangedCallback, CallbackImmediately);
}

void USettingsManager::RegisterFloatCVarSettingWithCallback(FName Name, float DefaultValue, const FString& Help, FFloatCVarChangedSignature ChangedCallback, bool CallbackImmediately)
{
	RegisterFloatCVarSetting(Name, DefaultValue, Help);
	UCVarChangeListenerManager::AddFloatCVarCallbackStatic(Name, ChangedCallback, CallbackImmediately);
}

void USettingsManager::RegisterStringCVarSettingWithCallback(FName Name, const FString& DefaultValue, const FString& Help, FStringCVarChangedSignature ChangedCallback, bool CallbackImmediately)
{
	RegisterStringCVarSetting(Name, DefaultValue, Help);
	UCVarChangeListenerManager::AddStringCVarCallbackStatic(Name, ChangedCallback, CallbackImmediately);
}

void USettingsManager::AutoDetectSettingsStatic()
{
	Get()->AutoDetectSettings();
}

void USettingsManager::SaveSettingStatic(FAutoSettingData SettingData)
{
	Get()->SaveSetting(SettingData, true);
}

void USettingsManager::ApplySettingStatic(FAutoSettingData SettingData)
{
	Get()->ApplySetting(SettingData);
}

USettingsManager::USettingsManager() : IniName("Settings"), SectionName("Settings")
{
	// Load Ini
	FConfigCacheIni::LoadGlobalIniFile(IniFilename, IniName.GetCharArray().GetData());
}

void USettingsManager::Init()
{
	ApplySettingsFromConfig();
}

FString USettingsManager::GetConfigValue(FName Key) const
{
	return GetConfigValue(Key, GetSection());
}

bool USettingsManager::HasConfigValue(FName Key) const
{
	FConfigSection* Section = GetSection();
	return Section && Section->Find(Key) != nullptr;
}

FString USettingsManager::GetConfigValue(FName Key, FConfigSection * Section) const
{
	if (Section)
	{
		FConfigValue* ConfigValue = Section->Find(Key);
		if (ConfigValue)
			return ConfigValue->GetValue();
	}

	return FString();
}

void USettingsManager::SaveSetting(FAutoSettingData SettingData, bool bApplySetting)
{
	if (!SettingData.Key.IsNone() && !SettingData.Value.IsEmpty())
	{
		if (bApplySetting)
		{
			ApplySetting(SettingData);
		}

		SetConfigValue(SettingData.Key, SettingData.Value);
		OnSettingSaved.Broadcast(SettingData);
	}
}

void USettingsManager::SetConfigValue(FName Key, FString Value)
{
	if (!Key.IsNone() && !Value.IsEmpty())
	{
		GConfig->SetString(SectionName.GetCharArray().GetData(), Key.ToString().GetCharArray().GetData(), Value.GetCharArray().GetData(), IniFilename);
		GConfig->Flush(false, IniFilename);
	}
}

void USettingsManager::ApplySetting(FAutoSettingData SettingData)
{
	if (UConsoleUtils::IsCVarRegistered(SettingData.Key))
		UConsoleUtils::SetStringCVar(SettingData.Key, SettingData.Value);
}

void USettingsManager::ApplySettingsFromConfig()
{
	FConfigSection* Section = GetSection();

	int32 SettingsLoaded = 0;

	if (Section)
	{
		TArray<FName> Keys;
		Section->GetKeys(Keys);

		for (const FName Key : Keys)
		{
			const FString Value = GetConfigValue(Key, Section);
			if (UConsoleUtils::IsCVarRegistered(Key))
			{
				UConsoleUtils::SetStringCVar(Key, Value);
				SettingsLoaded++;
			}
		}
	}

	UE_LOG(LogAutoSettings, Log, TEXT("Applied %i settings from config"), SettingsLoaded);
}

void USettingsManager::AutoDetectSettings()
{
	const Scalability::FQualityLevels State = Scalability::BenchmarkQualityLevels();
	Scalability::SetQualityLevels(State);

	// Save new scalability values to config
	// These are all the values that the Unreal scalability benchmark changes
	SaveSetting(FAutoSettingData("sg.ResolutionQuality", FString::FromInt(State.ResolutionQuality)), false);
	SaveSetting(FAutoSettingData("sg.ViewDistanceQuality", FString::FromInt(State.ViewDistanceQuality)), false);
	SaveSetting(FAutoSettingData("sg.AntiAliasingQuality", FString::FromInt(State.AntiAliasingQuality)), false);
	SaveSetting(FAutoSettingData("sg.ShadowQuality", FString::FromInt(State.ShadowQuality)), false);
	SaveSetting(FAutoSettingData("sg.PostProcessQuality", FString::FromInt(State.PostProcessQuality)), false);
	SaveSetting(FAutoSettingData("sg.TextureQuality", FString::FromInt(State.TextureQuality)), false);
	SaveSetting(FAutoSettingData("sg.EffectsQuality", FString::FromInt(State.EffectsQuality)), false);
	SaveSetting(FAutoSettingData("sg.FoliageQuality", FString::FromInt(State.FoliageQuality)), false);
}

FConfigSection * USettingsManager::GetSection() const
{
	return GConfig->GetSectionPrivate(SectionName.GetCharArray().GetData(), false, true, IniFilename);
}