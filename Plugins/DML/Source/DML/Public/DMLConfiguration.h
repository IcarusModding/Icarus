#pragma once
#include "CoreMinimal.h"

struct DML_API FDMLConfiguration {
public:
    /** Constructor setting configuration defaults */
    FDMLConfiguration();

    /**
    * Development mode is the mode in which DML will load
    * mods which are considered unsafe in normal environment
    * Most notable, it will be able to load raw .dll or .pak mods, which
    * is handy for development process because you avoid packing stuff every time you want to test it
    * Note that such mods CANNOT have dependencies or dependents, and are always loaded in the last order
    * Environment with enabled development mode is invalid for submitting bug reports
    */
    bool bDevelopmentMode;

    /**
    * Opens a console window wich outputs all standard output streams
    * for allowing you to better debug the runtime
    */
    bool bConsoleWindow;

    /**
    * List of fully qualified chat command names that won't be usable by players in the game
    * Full command name is mod_reference:command_name
    * Note that this setting will only affect players, other command senders will still be able to use it
    */
    TArray<FString> DisabledChatCommands;

    /**
    * Whenever to enable console commands which are considered cheat and disabled by default
    * See UICheatManager for command list
    */
    bool bEnableCheatConsoleCommands;
public:
    /** Deserializes configuration from JSON object */
    static void ReadFromJson(const TSharedPtr<class FJsonObject>& Json, FDMLConfiguration& OutConfiguration, bool* OutIsMissingSections = NULL);

    /** Serializes configuration into JSON object */
    static void WriteToJson(const TSharedPtr<class FJsonObject>& OutJson, const FDMLConfiguration& Configuration);
};