#pragma once
#include "CoreMinimal.h"
#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "RRunStateComp.generated.h"

class URCharacterMovementComponent;
UCLASS( Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent) )
class RATSIMULATOR_API URRunStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URRunStateComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	URCharacterMovementComponent* RCharacterMovementComponent;

public:
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;
};
