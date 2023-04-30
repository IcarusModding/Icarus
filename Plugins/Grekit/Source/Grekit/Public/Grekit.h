#pragma once
#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "GrekitSettings.h"
#include "AssetData.h"
#include "Dom/JsonObject.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrekit, Verbose, All);

class FGrekitModule : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
private:
    TSharedPtr<class FUICommandList> PluginCommands;
    
    void RegisterSettings() const;
    void UnregisterSettings() const;
};
