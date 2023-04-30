#include "GrekitCommands.h"
#include "GrekitStyle.h"

#define LOCTEXT_NAMESPACE "FGrekitModule"

FGrekitCommands::FGrekitCommands() : TCommands<FGrekitCommands>(
    TEXT("Grekit"),
    NSLOCTEXT("Contexts", "Grekit", "Grekit Plugin"),
    NAME_None,
    FGrekitStyle::GetStyleSetName()) {}

void FGrekitCommands::RegisterCommands() {
    UI_COMMAND(OpenPluginWindow, "Grekit", "Open the grekit window", EUserInterfaceActionType::Button, FInputChord{});
}

#undef LOCTEXT_NAMESPACE
