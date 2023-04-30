﻿#pragma once
#include "Configuration/ConfigProperty.h"
#include "ConfigPropertyInteger.generated.h"

UCLASS()
class DML_API UConfigPropertyInteger : public UConfigProperty {
    GENERATED_BODY()
public:
    /** Current value of this configuration property */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration Property")
    int32 Value;
    
    UConfigPropertyInteger();
  
	//Begin UConfigProperty
	virtual FString DescribeValue_Implementation() const override;
	virtual URawFormatValue* Serialize_Implementation(UObject* Outer) const override;
	virtual void Deserialize_Implementation(const URawFormatValue* Value) override;
    virtual FConfigVariableDescriptor CreatePropertyDescriptor_Implementation(UConfigGenerationContext* Context, const FString& OuterPath) const override;
    virtual void FillConfigStruct_Implementation(const FReflectedObject& ReflectedObject, const FString& VariableName) const override;
	//End UConfigProperty
};
