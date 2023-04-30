#pragma once
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FGrekitCommands : public TCommands<FGrekitCommands> {
public:
    FGrekitCommands();

    virtual void RegisterCommands() override;
public:
    TSharedPtr<FUICommandInfo> OpenPluginWindow;
};