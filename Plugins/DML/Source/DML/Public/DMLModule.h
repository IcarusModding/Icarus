#pragma once

extern "C" DLLEXPORT IModuleInterface* InitializeModule();

class DML_API FDMLModule : public FDefaultGameModuleImpl {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};