// Copyright Sam Bonifacio. All Rights Reserved.

#include "AutoSettings.h"
#include "SettingsManager.h"
#include "InputMapping/InputMappingManager.h"
#include "Misc/AutoSettingsConfig.h"
#include "Misc/CoreDelegates.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FAutoSettingsModule"

DEFINE_LOG_CATEGORY(LogAutoSettings);

void FAutoSettingsModule::StartupModule()
{
	UE_LOG(LogAutoSettings, Log, TEXT("AutoSettings module starting up"));

#if WITH_EDITOR

	// Register project-specific plugin settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "AutoSettings",
			LOCTEXT("AutoSettingsName", "AutoSettings"),
			LOCTEXT("AutoSettingsDescription", "Configure the AutoSettings plugin."),
			GetMutableDefault<UAutoSettingsConfig>()
			);
	}
#endif

	FCoreDelegates::OnPostEngineInit.AddLambda([]()
	{
		// Initialize settings manager so that it can load config
		// Do this after engine init so that saved CVar values apply on top of engine-determined values
		// The module itself has to load before PostEngineInit so that content is available to the game
		USettingsManager::Get()->Init();
		UInputMappingManager::Get();
	});

}

void FAutoSettingsModule::ShutdownModule()
{
#if WITH_EDITOR

	// Unregister plugin settings
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "AutoSettings"	);
	}
#endif

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutoSettingsModule, AutoSettings)
