#include "Util/BlueprintLoggingLibrary.h"
#include "Icarus.h"
#include "DaedalusModLoader.h"
#include "UObject/Package.h"
#include "Util/BlueprintAssetHelperLibrary.h"

DEFINE_LOG_CATEGORY(LogBlueprintLogging);

FLogCategoryBase* CreateOrFindLogCategory(const FName LogCategoryName, ELogVerbosity::Type DefaultVerbosity) {
	static TMap<FName, TSharedPtr<FLogCategoryBase>> RegisteredCategories;

	if (const TSharedPtr<FLogCategoryBase>* Existing = RegisteredCategories.Find(LogCategoryName)) {
		return Existing->Get();
	}

	UE_LOG(LogDaedalusModLoader, Log, TEXT("Registering dynamic Blueprint logging category %s with default verbosity %s"),
		*LogCategoryName.ToString(), FOutputDeviceHelper::VerbosityToString(DefaultVerbosity));

	const TSharedPtr<FLogCategoryBase> NewCategory = MakeShared<FLogCategoryBase>(LogCategoryName, DefaultVerbosity, ELogVerbosity::All);
	RegisteredCategories.Add(LogCategoryName, NewCategory);

	return NewCategory.Get();
}

void UBlueprintLoggingLibrary::LogImpl(ELogVerbosity::Type Verbosity, UPackage* SourcePackage, const FString& Message) {
	const FString PluginName = UBlueprintAssetHelperLibrary::FindPluginNameByObjectPath(SourcePackage->GetName());

	FLogCategoryBase* TargetLogCategory = NULL;

	if (PluginName == TEXT("Icarus")) {
		UE_LOG(LogDaedalusModLoader, Warning, TEXT("UBlueprintLoggingLibrary is used from the Icarus-owned package %s"), *SourcePackage->GetName());
		TargetLogCategory = &LogTemp;

	}
	else if (PluginName == TEXT("DML")) {
		//DML is non-conforming because it's logging category does not match it's plugin name + Log prefix
		TargetLogCategory = &LogDaedalusModLoader;

	}
	else if (PluginName.IsEmpty()) {
		UE_LOG(LogDaedalusModLoader, Error, TEXT("UBlueprintLoggingLibrary is used from a package %s belonging to an unknown plugin"), *SourcePackage->GetName());
		TargetLogCategory = &LogBlueprintLogging;

	}
	else {
		//Grekit -> LogGrekit
		const FString LogCategoryName = FString::Printf(TEXT("Log%s"), *PluginName);
		const ELogVerbosity::Type DefaultVerbosity = LogBlueprintLogging.GetVerbosity();

		TargetLogCategory = CreateOrFindLogCategory(*LogCategoryName, DefaultVerbosity);
	}
	check(TargetLogCategory);

	if (Verbosity == ELogVerbosity::Fatal || !TargetLogCategory->IsSuppressed(Verbosity)) {
		FMsg::Logf(UE_LOG_SOURCE_FILE(__FILE__), __LINE__, TargetLogCategory->GetCategoryName(), Verbosity, TEXT("[%s]: %s"), *SourcePackage->GetPathName(), *Message);
	}
}
