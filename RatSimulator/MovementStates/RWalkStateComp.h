#pragma once
#include "CoreMinimal.h"
#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "RWalkStateComp.generated.h"

class URCharacterMovementComponent;

UCLASS( Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent) )
class RATSIMULATOR_API URWalkStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:	
	URWalkStateComp();

private:
	UPROPERTY()
	URCharacterMovementComponent* RCharacterMovementComponent;

	UPROPERTY()
	bool bTick = false;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:	
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;
};
