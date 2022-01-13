#pragma once

#include "CoreMinimal.h"

#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"

#include "RCrouchStateComp.generated.h"


class UCameraComponent;
class UTimelineComponent;
class URCharacterMovementComponent;
class ARCharacter;
UCLASS( Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent) )
class RATSIMULATOR_API URCrouchStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* TimelineComponent;

	UPROPERTY()
	float CameraZOffsetIntent = 0.f;
	
private:
	FOnTimelineFloat CameraZOffsetTimelineFloat;
	UFUNCTION()
	void CameraZoffsetTimeline(float Alpha);

	FOnTimelineFloat JumpChargeTimelineFloat;
	UFUNCTION()
	void JumpChargeTimeline(float Output);

	FOnTimelineFloat JumpChargeUpgradeInfluenceTimelineFloat;
	UFUNCTION()
	void JumpChargeUpgradeInfluenceTimeline(float Output);

	float JumpChargeUpgradeInfluence = 0.f;
	
public:
	URCrouchStateComp();

	UPROPERTY(BlueprintReadOnly)
	ARCharacter* RCharacter;

	UPROPERTY(BlueprintReadOnly)
	URCharacterMovementComponent* RCharacterMovementComponent;

	UPROPERTY()
	UCameraComponent* RCharacterCamera;

private:
	UPROPERTY()
	float JumpCharge = 0.f;

	UPROPERTY()
	bool bHasJumpedSinceLastGrounded = false;
	
	UPROPERTY()
	float CoyoteTimer = 0.f;
	
	bool CanJump() const;

	
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;
};


/// todo: WITH COLLISION

// bool ARCharacter::CanStandUp() const
// {
// 	FHitResult HitResult;
//
// 	const float CurrentCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
// 	// may be scaled since it will scale down while crouching
//
// 	const FVector GroundLocation = GetCapsuleComponent()->GetComponentLocation() + (-FVector::ZAxisVector *
// 		CurrentCapsuleHalfHeight);
// 	const FVector TopOfCapsuleLocation = GroundLocation + (FVector::ZAxisVector * StandingCapsuleHalfHeight * 2);
//
//
// 	FCollisionQueryParams QueryParams;
// 	QueryParams.AddIgnoredActor(this);
//
// 	bool bFullCapsuleWouldNotFit = GetWorld()->LineTraceSingleByChannel(HitResult, GroundLocation, TopOfCapsuleLocation,
// 																		ECC_Visibility, QueryParams);
//
// 	return !bFullCapsuleWouldNotFit;
// }