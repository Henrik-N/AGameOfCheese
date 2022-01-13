#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RStateInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class URStateInterface : public UInterface
{
	GENERATED_BODY()
};

class RATSIMULATOR_API IRStateInterface
{
	GENERATED_BODY()
public:
	/// ENABLE
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void OnEnableState();
	
	virtual void OnEnableState_Implementation();

	/// Disable
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void OnDisableState(const TScriptInterface<IRStateInterface>& UpcomingState);
	
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState);
};
