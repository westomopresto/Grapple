// Copyright 2019 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSimpleSteamLeaderboardsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
