// Copyright Sam Bonifacio. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAutoSettings, Log, All);

class FAutoSettingsModule : public IModuleInterface
{
public:

	// Static get module
	static inline FAutoSettingsModule* Get()
	{
		return FModuleManager::LoadModulePtr< FAutoSettingsModule >("AutoSettings");
	}

protected:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};