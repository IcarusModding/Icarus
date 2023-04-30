#include "Module/WorldModuleManager.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DaedalusModLoader.h"
#include "ModLoading/PluginModuleLoader.h"
#include "Module/GameWorldModule.h"
#include "Module/MenuWorldModule.h"

UWorldModule* UWorldModuleManager::FindModule(const FName& ModReference) const {
    UWorldModule* const* WorldModule = RootModuleMap.Find(ModReference);
    if (WorldModule != NULL) {
        return *WorldModule;
    }
    return NULL;
}

void UWorldModuleManager::WaitForGameState(FLatentActionInfo& LatentInfo) {
	
}

bool UWorldModuleManager::ShouldCreateSubsystem(UObject* Outer) const {
	UWorld* WorldOuter = CastChecked<UWorld>(Outer);
	return FPluginModuleLoader::ShouldLoadModulesForWorld(WorldOuter);
}

void UWorldModuleManager::Initialize(FSubsystemCollectionBase& Collection) {
	// TODO
	
	UWorld* OuterWorld = GetWorld();
	OuterWorld->OnActorsInitialized.AddUObject(this, &UWorldModuleManager::InitializeModules);
	OuterWorld->OnWorldBeginPlay.AddUObject(this, &UWorldModuleManager::PostInitializeModules);
}

void UWorldModuleManager::InitializeModules(const UWorld::FActorsInitializedParams&) {
	// TODO
	
	//We cannot construct modules earlier because before that moment world just lacks
	//information about it's type and authority game mode object on host
	ConstructModules();

	DispatchLifecycleEvent(ELifecyclePhase::INITIALIZATION);
	NotifyContentRegistry();
}

void UWorldModuleManager::PostInitializeModules() {
	DispatchLifecycleEvent(ELifecyclePhase::POST_INITIALIZATION);
}

void UWorldModuleManager::NotifyContentRegistry() {
	// TODO
}

void UWorldModuleManager::ConstructModules() {
    //Use game world module by default
    TSubclassOf<UWorldModule> ModuleTypeClass = UGameWorldModule::StaticClass();

    //Use MenuWorldModule if we are in the main menu
    if (FPluginModuleLoader::IsMainMenuWorld(GetWorld())) {
        ModuleTypeClass = UMenuWorldModule::StaticClass();
    }

    //Discover modules of the relevant types
    const TArray<FDiscoveredModule> DiscoveredModules = FPluginModuleLoader::FindRootModulesOfType(ModuleTypeClass);

    TMap<FString, FString> AlreadyLoadedMods;
    for (const FDiscoveredModule& Module : DiscoveredModules) {
        
        //Make sure we are not trying to register a single plugin twice
        if (AlreadyLoadedMods.Contains(Module.OwnerPluginName)) {
            const FString OtherModulePath = AlreadyLoadedMods.FindChecked(Module.OwnerPluginName);
            UE_LOG(LogDaedalusModLoader, Fatal, TEXT("Failed to initialize World Module for Plugin %s: "
                "Multiple root modules have been discovered, %s and %s. Only one root module is allowed"),
                *Module.OwnerPluginName, *OtherModulePath, *Module.ModuleClass->GetPathName());
            continue;
        }

        //Register module in the game instance module manager now
        const TSubclassOf<UWorldModule> WorldModule = Module.ModuleClass.Get();
        AlreadyLoadedMods.Add(Module.OwnerPluginName, Module.ModuleClass->GetPathName());
        CreateRootModule(*Module.OwnerPluginName, WorldModule);
    }
    
    UE_LOG(LogDaedalusModLoader, Log, TEXT("Discovered %d world modules of class %s"), AlreadyLoadedMods.Num(), *ModuleTypeClass->GetName());
    
    //Dispatch construction lifecycle event
    DispatchLifecycleEvent(ELifecyclePhase::CONSTRUCTION);
}

void UWorldModuleManager::CreateRootModule(const FName& ModReference, TSubclassOf<UWorldModule> ObjectClass) {
    //Allocate module object and set mod reference
    UWorldModule* RootWorldModule = NewObject<UWorldModule>(this, ObjectClass, ModReference);
    check(RootWorldModule);
    RootWorldModule->SetOwnerModReference(ModReference);
    
    //Add it to internal lists
    RootModuleMap.Add(ModReference, RootWorldModule);
    RootModuleList.Add(RootWorldModule);
}

void UWorldModuleManager::DispatchLifecycleEvent(ELifecyclePhase Phase) {
    //Notify log of our current loading phase, in case of things going wrong
    UE_LOG(LogDaedalusModLoader, Log, TEXT("Dispatching lifecycle event %s to world %s modules"), 
        *UModModule::LifecyclePhaseToString(Phase), *GetWorld()->GetMapName());
    
    //Iterate modules in their order of registration and dispatch lifecycle event to them
    for (UWorldModule* RootModule : RootModuleList) {
        RootModule->DispatchLifecycleEvent(Phase);
    }
}

void FWaitForGameStateLatentAction::UpdateOperation(FLatentResponse& Response) {
	UWorld* WorldObject = TargetWorld.Get();
	bool bHasCompleted = false;

	// TODO

	Response.FinishAndTriggerIf(bHasCompleted, ExecutionFunction, OutputLink, CallbackTarget);
}

#if WITH_EDITOR
FString FWaitForGameStateLatentAction::GetDescription() const {
	return TEXT("Wait For GameState");
}
#endif
