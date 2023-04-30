#include "GrekitSettings.h"

UGrekitSettings* UGrekitSettings::Get() {
    return GetMutableDefault<UGrekitSettings>();
}

void UGrekitSettings::SaveSettings() {
    SaveConfig();
}
