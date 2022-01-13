#pragma once

#include "CoreMinimal.h"

#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "RVaultStateComp.generated.h"


class URDA_VaultingSettings;
class URVaultingComponent;
class URCharacterMovementComponent;

UCLASS( Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent) )
class RATSIMULATOR_API URVaultStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:	
	URVaultStateComp();

	UPROPERTY()
	URCharacterMovementComponent* RCharacterMovementComponent;

//Specific Vaulting variables
protected:
	UPROPERTY()
	URVaultingComponent* VaultingComponent;
	UPROPERTY()
	URDA_VaultingSettings* DataSettings;
	
	UPROPERTY(BlueprintReadOnly)
	float LerpTimer = 0.0f;
	float VaultStartSpeedPercent = 1.f;
	FVector VaultStartVelocity;
	
	FVector VaultStartPos;
	
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;

protected:
	void MoveTowardsDestination(float DeltaTime);
	void MoveBezier(float DeltaTime);

	//Utility
	float InverseLerp(float A, float B, float Value) const;
	FVector Bezier3Points(const FVector& A, const FVector& B, const FVector& C, float T) const;
};
