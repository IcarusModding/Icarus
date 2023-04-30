// Copyright RocketWerkz Studios Ltd. All Rights Reserved.

#pragma once

#include "Icarus.h"
#include "Modules/ModuleManager.h"

class FIcarusModule : public FDefaultGameModuleImpl
{
public:
	/**
	 * Called when the module is loaded into memory
	 */
	virtual void StartupModule() override;

	/**
	 * Called when the module is unloaded from memory
	 */
	virtual void ShutdownModule() override;

	/** We want this module to be hot reloaded */
	virtual bool IsGameModule() const override { return true; }
};