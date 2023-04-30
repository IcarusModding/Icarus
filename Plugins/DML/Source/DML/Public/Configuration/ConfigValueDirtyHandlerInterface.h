#pragma once
#include "CoreMinimal.h"
#include "ConfigValueDirtyHandlerInterface.generated.h"

/** Blueprint wrapper for the IUConfigValueDirtyHandler interface */
UINTERFACE(Blueprintable)
class DML_API UConfigValueDirtyHandlerInterface : public UInterface {
	GENERATED_BODY()
};

/**
* Used to mark configuration value dirty.
* First instance of the Outer object implementing this interface is selected
* upon UConfigValue.MarkDirty() call and then called to process
* marking configuration dirty
*/
class DML_API IConfigValueDirtyHandlerInterface {
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintNativeEvent)
    void HandleMarkDirty();
};
