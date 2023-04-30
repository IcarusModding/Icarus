#include "Module/GameWorldModule.h"

void UGameWorldModule::DispatchLifecycleEvent(ELifecyclePhase Phase) {
    //Register default content before calling blueprint event logic
	if (Phase == ELifecyclePhase::CONSTRUCTION) {
		RegisterConstructionPhaseContent();
	}
	
    if (Phase == ELifecyclePhase::INITIALIZATION) {
        RegisterDefaultContent();
    }
    Super::DispatchLifecycleEvent(Phase);
}

void UGameWorldModule::RegisterConstructionPhaseContent() {
	// TODO
}

void UGameWorldModule::RegisterDefaultContent() {
    // TODO
}
