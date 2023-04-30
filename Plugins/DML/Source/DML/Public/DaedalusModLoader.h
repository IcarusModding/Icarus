#pragma once
#include "DMLConfiguration.h"
#include "Util/SemVersion.h"

// TODO : Move to the ModLoading later
#define ICARUS_MOD_NAME TEXT("Icarus")

DECLARE_LOG_CATEGORY_EXTERN(LogDaedalusModLoader, Log, All);

/** Describes a set of known attributes returned by GetExtraAttributes() */
struct DML_API FModLoaderExtraAttributes {
public:
};

class DML_API FDaedalusModLoader {
private:
	//Delete all the constructors, all members of this class are static
	FDaedalusModLoader() = delete;
	FDaedalusModLoader(const FDaedalusModLoader&) = delete;
	FDaedalusModLoader(FDaedalusModLoader&&) = delete;
public:
	/** Returns version of the mod loader */
	static FVersion GetModLoaderVersion();

	/** Returns map of implementation dependent extra attributes describing current mod loader configuration */
	static TMap<FName, FString> GetExtraAttributes();
	
	/** Returns active DML configuration. If not loaded, it will return empty struct */
	FORCEINLINE static FDMLConfiguration GetDMLConfiguration() { return DMLConfigurationPrivate; }
private:
	friend class FDMLModule;
	
	// TODO: static TSharedPtr<FModHandler> ModHandlerPrivate;
	static FDMLConfiguration DMLConfigurationPrivate;

	/** Loads DML configuration from file and optionally saves it back */
	static void LoadDMLConfiguration(bool bAllowSave);

	/** Checks game and bootstrapper version before launching */
	static void CheckGameVersion();

	/** Registers DML subsystems */
	static void RegisterSubsystemPatches();

	/** Registers global DML subsystems that are registered only once */
	static void RegisterSubsystems();

	/** Does early mod loading initialization like allocating mod handler, discovering mods and loading DLLs */
	static void PreInitializeModLoading();

	/** Finishes mod loading and mounts packages, initializes subsystems, loads mod configurations, etc */
	static void InitializeModLoading();
};
