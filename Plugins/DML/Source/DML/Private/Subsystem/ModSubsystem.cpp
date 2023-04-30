#include "Subsystem/ModSubsystem.h"

UModSubsystem::UModSubsystem() {
	this->bInitDispatched = false;
}

void UModSubsystem::DispatchInit() {
	if (!bInitDispatched) {
		Init();
		this->bInitDispatched = true;
	}
}
