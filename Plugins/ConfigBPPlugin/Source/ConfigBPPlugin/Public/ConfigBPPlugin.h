// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FConfigBPPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};