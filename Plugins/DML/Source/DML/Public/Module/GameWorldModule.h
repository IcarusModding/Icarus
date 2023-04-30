#pragma once
#include "CoreMinimal.h"
#include "Module/WorldModule.h"
#include "GameWorldModule.generated.h"

/** World module initialized for normal game world only (e.g skipped for menu world) */
UCLASS(Blueprintable)
class DML_API UGameWorldModule : public UWorldModule {
    GENERATED_BODY()
public:
    /** Register content from properties here */
    virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;
protected:
	/** Registers content at the construction phase */
	void RegisterConstructionPhaseContent();
	
    /** Registers default content from properties specified above */
    void RegisterDefaultContent();
};
