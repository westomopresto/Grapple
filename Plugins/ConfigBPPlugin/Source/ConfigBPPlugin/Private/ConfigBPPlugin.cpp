// Copyright 2017-2018 HowToCompute. All Rights Reserved.

#include "ConfigBPPlugin.h"

#define LOCTEXT_NAMESPACE "FConfigBPPluginModule"

void FConfigBPPluginModule::StartupModule()
{
	// We don't need to do anything when this module starts up -> plugin loads (yet).
}

void FConfigBPPluginModule::ShutdownModule()
{
	// We don't need to do anything when this module shuts down -> plugin unloads (yet).
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FConfigBPPluginModule, ConfigBPPlugin)