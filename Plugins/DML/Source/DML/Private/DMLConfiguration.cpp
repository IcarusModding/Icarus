#include "DMLConfiguration.h"
#include "Dom/JsonObject.h"

FDMLConfiguration::FDMLConfiguration() :
    bDevelopmentMode(false),
    bConsoleWindow(false),
    bEnableCheatConsoleCommands(false) {
}

void FDMLConfiguration::ReadFromJson(const TSharedPtr<FJsonObject>& Json, FDMLConfiguration& OutConfiguration, bool* OutIsMissingSections) {
    bool bIsMissingSectionsInternal = false;
    
    if (Json->HasTypedField<EJson::Boolean>(TEXT("developmentMode"))) {
        OutConfiguration.bDevelopmentMode = Json->GetBoolField(TEXT("developmentMode"));
    } else {
        bIsMissingSectionsInternal = true;
    }
    
    if (Json->HasTypedField<EJson::Boolean>(TEXT("consoleWindow"))) {
        OutConfiguration.bConsoleWindow = Json->GetBoolField(TEXT("consoleWindow"));
    } else {
        bIsMissingSectionsInternal = true;
    }
    
    if (Json->HasTypedField<EJson::Boolean>(TEXT("enableCheatConsoleCommands"))) {
        OutConfiguration.bEnableCheatConsoleCommands = Json->GetBoolField(TEXT("enableCheatConsoleCommands"));
    } else {
        bIsMissingSectionsInternal = true;
    }
    
    if (Json->HasTypedField<EJson::Array>(TEXT("disabledChatCommands"))) {
        const TArray<TSharedPtr<FJsonValue>>& DisabledChatCommands = Json->GetArrayField(TEXT("disabledChatCommands"));
        for (const TSharedPtr<FJsonValue>& Value : DisabledChatCommands) {
            if (Value->Type == EJson::String)
                OutConfiguration.DisabledChatCommands.Add(Value->AsString());
        }
    } else {
        bIsMissingSectionsInternal = true;
    }

    if (OutIsMissingSections) {
        *OutIsMissingSections = bIsMissingSectionsInternal;
    }
}

void FDMLConfiguration::WriteToJson(const TSharedPtr<FJsonObject>& OutJson, const FDMLConfiguration& Configuration) {
    OutJson->SetBoolField(TEXT("developmentMode"), Configuration.bDevelopmentMode);
    OutJson->SetBoolField(TEXT("consoleWindow"), Configuration.bConsoleWindow);
    OutJson->SetBoolField(TEXT("enableCheatConsoleCommands"), Configuration.bEnableCheatConsoleCommands);

    TArray<TSharedPtr<FJsonValue>> DisabledChatCommands;
    for (const FString& Value : Configuration.DisabledChatCommands) {
        DisabledChatCommands.Add(MakeShareable(new FJsonValueString(Value)));
    }
    OutJson->SetArrayField(TEXT("disabledChatCommands"), DisabledChatCommands);
}
