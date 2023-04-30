#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GrekitSettings.generated.h"

UENUM()
enum class EGrekitStartGameType : uint8 {
    NONE UMETA(DisplayName = "Do not launch"),
    STEAM UMETA(DisplayName = "Steam"),
    EPIC_EARLY_ACCESS UMETA(DisplayName = "Epic: Early Access"),
    EPIC_EXPERIMENTAL UMETA(DisplayName = "Epic: Experimental")
};

UCLASS(config=Game)
class GREKIT_API UGrekitSettings : public UObject {
    GENERATED_BODY()
public:
    /** Retrieves global instance of grekit settings */
    static UGrekitSettings* Get();

    /** Saves grekit settings to configuration file */
    void SaveSettings();

    UPROPERTY(EditAnywhere, config, Category = Config)
    FDirectoryPath IcarusGamePath;

    UPROPERTY(EditAnywhere, config, Category = Config)
    EGrekitStartGameType LaunchGameAfterPacking;

    UPROPERTY(EditAnywhere, config, Category = Config)
    bool bCopyModsToGame = false;

    UPROPERTY(BlueprintReadOnly, config, Category = Config)
    TMap<FString, bool> ModSelection;
};
