// Copyright (C) Alexandre Depail. 2019. All Rights Reserved.
#pragma once

#include "Modules/ModuleManager.h"

class EasyColorPickerImpl : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};
