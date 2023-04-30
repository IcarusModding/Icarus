#include "DMLModule.h"
#include "DaedalusModLoader.h"

void FDMLModule::StartupModule() {
	//Basic subsystems like logging are initialized on OnInit
	FDaedalusModLoader::PreInitializeModLoading();
        
	//UObject subsystem and Engine are initialized on PostEngineInit and we need to delay their initialization to that moment
	FCoreDelegates::OnPostEngineInit.AddStatic(FDaedalusModLoader::InitializeModLoading);
}

void FDMLModule::ShutdownModule() {
}

IMPLEMENT_GAME_MODULE(FDMLModule, DML);
