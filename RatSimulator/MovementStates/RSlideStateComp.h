#pragma once

#include "CoreMinimal.h"

#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "RSlideStateComp.generated.h"


class ARCharacter;
class URCharacterMovementComponent;
UCLASS( Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent) )
class RATSIMULATOR_API URSlideStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:
	URSlideStateComp();

	UPROPERTY(BlueprintReadOnly)
	ARCharacter* RCharacter;
	
	UPROPERTY(BlueprintReadOnly)
	URCharacterMovementComponent* RCharacterMovementComponent;

private:
	UPROPERTY()
	float DefaultGroundFriction;

	UPROPERTY()
	float DefaultBrakingDeceleration;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SetSlideVelocity();
	void SetGroundFrictionAndDeceleration();
	void ResetGroundFrictionAndDeceleration();
public:
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;
};
