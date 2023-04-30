#include "DaedalusModLoader.h"
//#include "IPlayerController.h"
#include "Configuration/ConfigManager.h"

#ifndef DML_BUILD_METADATA
#define DML_BUILD_METADATA "unknown"
#endif

extern "C" DLLEXPORT const TCHAR* modLoaderVersionString = TEXT("1.0.0+") TEXT(DML_BUILD_METADATA);
extern "C" DLLEXPORT const long targetGameVersion = 208572;

DEFINE_LOG_CATEGORY(LogDaedalusModLoader);

// TSharedPtr<FModHandler> FDaedalusModLoader::ModHandlerPrivate = NULL;
FDMLConfiguration FDaedalusModLoader::DMLConfigurationPrivate;

FVersion FDaedalusModLoader::GetModLoaderVersion() {
    static FVersion* ModLoaderVersion = NULL;
    if (ModLoaderVersion == NULL) {
        FString OutErrorMessage;
        ModLoaderVersion = new FVersion();
        ModLoaderVersion->ParseVersion(modLoaderVersionString, OutErrorMessage);
    }
    return *ModLoaderVersion;
}

TMap<FName, FString> FDaedalusModLoader::GetExtraAttributes() {
    TMap<FName, FString> OutExtraAttributes;
    return OutExtraAttributes;
}

void FDaedalusModLoader::LoadDMLConfiguration(bool bAllowSave) {
    const FString ConfigLocation = UConfigManager::GetConfigurationFilePath(FConfigId{TEXT("DML")});
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    bool bShouldWriteConfiguration = false;
    
    if (PlatformFile.FileExists(*ConfigLocation)) {
        FString RawDMLConfiguration;
        if (FFileHelper::LoadFileToString(RawDMLConfiguration, *ConfigLocation)) {
            
            TSharedPtr<FJsonObject> OutJsonObject;
            const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawDMLConfiguration);
            
            if (FJsonSerializer::Deserialize(JsonReader, OutJsonObject)) {
                FDMLConfiguration::ReadFromJson(OutJsonObject, DMLConfigurationPrivate, &bShouldWriteConfiguration);
                UE_LOG(LogDaedalusModLoader, Display, TEXT("Successfully loaded DML configuration from disk"));
                
            } else {
                UE_LOG(LogDaedalusModLoader, Warning, TEXT("Failed to load DML configuration, JSON is malformed"));
                bShouldWriteConfiguration = true;
            }
        } else {
            UE_LOG(LogDaedalusModLoader, Error, TEXT("Failed to load DML configuration from %s"), *ConfigLocation);   
        }
    } else {
        UE_LOG(LogDaedalusModLoader, Display, TEXT("DML configuration file is missing, saving new one"));
        bShouldWriteConfiguration = true;
    }

    if (bShouldWriteConfiguration) {
        const TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
        FDMLConfiguration::WriteToJson(JsonObject, DMLConfigurationPrivate);

        FString OutSerializedConfiguration;
        const TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutSerializedConfiguration);
        FJsonSerializer::Serialize(JsonObject, JsonWriter);

        //Make sure configuration directory exists
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FPaths::GetPath(ConfigLocation));

        //Write file onto the disk now
        if (FFileHelper::SaveStringToFile(OutSerializedConfiguration, *ConfigLocation)) {
            UE_LOG(LogDaedalusModLoader, Display, TEXT("Successfully saved DML configuration"));
        } else {
            UE_LOG(LogDaedalusModLoader, Error, TEXT("Failed to save DML configuration to %s"), *ConfigLocation);
        }
    }
}

void FDaedalusModLoader::CheckGameVersion() {
    const uint32 CurrentChangelist = FEngineVersion::Current().GetChangelist();
    const uint32 MinChangelistSupported = (uint32) targetGameVersion;
    
    if (!(CurrentChangelist >= MinChangelistSupported)) {
        UE_LOG(LogDaedalusModLoader, Fatal, TEXT("Game version check failed: Game version is %d, but this DML version is built for %d"), CurrentChangelist, MinChangelistSupported);
    }
    UE_LOG(LogDaedalusModLoader, Display, TEXT("Version check passed successfully! Game Changelist: %d"), CurrentChangelist);
}

void FDaedalusModLoader::RegisterSubsystemPatches() {
    
}

void FDaedalusModLoader::RegisterSubsystems() {

}

void FDaedalusModLoader::PreInitializeModLoading() {
    UE_LOG(LogDaedalusModLoader, Display, TEXT("Daedalus Mod Loader v.%s pre-initializing..."), modLoaderVersionString);
    UE_LOG(LogDaedalusModLoader, Display, TEXT("Build Date: %s %s"), ANSI_TO_TCHAR(__DATE__), ANSI_TO_TCHAR(__TIME__));

    //Don't try to save configuration in the editor, because it will make new folders for no real reason
    const bool bAllowSavingConfiguration = !WITH_EDITOR;
    LoadDMLConfiguration(bAllowSavingConfiguration);

    //Check game version only on shipping platform, because in editor builds
    //changelist number is not actually correctly set, since it is built from scratch
    if (FPlatformProperties::RequiresCookedData()) {
        CheckGameVersion();
    }

    //Show console if we have been asked to in configuration
    //Console can also be shown using -LOG command line switch
    if (GetDMLConfiguration().bConsoleWindow) {
        GLogConsole->Show(true);
    }

    UE_LOG(LogDaedalusModLoader, Display, TEXT("Pre-initialization finished!"));
}

void FDaedalusModLoader::InitializeModLoading() {
    UE_LOG(LogDaedalusModLoader, Display, TEXT("Performing mod loader initialization"));

    //Install patches, but only do it in shipping for now because most of them involve Daedalus code and
    //we currently do not have FG code available in the editor
    if (FPlatformProperties::RequiresCookedData()) {
        UE_LOG(LogDaedalusModLoader, Display, TEXT("Registering subsystem patches..."));
        RegisterSubsystemPatches();
    }
    
    //Setup DML subsystems and custom content registries
    UE_LOG(LogDaedalusModLoader, Display, TEXT("Registering global subsystems..."));
    RegisterSubsystems();

    UE_LOG(LogDaedalusModLoader, Display, TEXT("Initialization finished!"));
}
