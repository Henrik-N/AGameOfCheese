#pragma once

#include "CoreMinimal.h"

#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "RClimbStateComp.generated.h"


class UCameraComponent;
USTRUCT()
struct FRSurfaceInfo3
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector SurfaceNormal = FVector::UpVector;

	UPROPERTY()
	float DistanceToSurface = 0.f;

	UPROPERTY()
	bool bIsEmpty = false;
	
	static FRSurfaceInfo3 CreateEmpty()
	{
		FRSurfaceInfo3 SurfaceInfo;
		SurfaceInfo.bIsEmpty = true;
		return std::move(SurfaceInfo);
	};
};


class ARCharacter;
class URCharacterMovementComponent;
UCLASS( Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent) )
class RATSIMULATOR_API URClimbStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:	
	URClimbStateComp();

	UPROPERTY(BlueprintReadOnly)
	ARCharacter* RCharacter;

	UPROPERTY()
	bool bTick = false;

	UPROPERTY(BlueprintReadOnly)
	URCharacterMovementComponent* RCharacterMovementComponent;

	UPROPERTY()
	bool bFacingWall = false;
	
	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* RCharacterCamera;

	UPROPERTY()
	FVector ClimbSurfaceNormal = FVector::ZAxisVector;
	
protected:

	virtual void BeginPlay() override;
	TOptional<FRSurfaceInfo3> FindSurfaceFromDirectionFromActor(AActor* Actor, const FVector& DirectionFromActor);

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Leap();
	
private:
	UPROPERTY()
	float TimeSinceClimbNormalUpdated = 0.f;


	UFUNCTION()
	void OnPawnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	void FollowWall(float DeltaTime);
	
	void DebugDrawLeapPredict(float DeltaTime);
	bool IsSurfaceVertical(const FVector& SurfaceNormal);
	bool ShouldStopClimbingSurface(const FVector& WallNormal);
	
	bool IsFacingWall(const FVector& WallNormal);
	
	void SetDefaultGravityEnabled(bool bEnabled);
	void StopVelocity();
	void ResetActorRotation();
	
public:
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;
};
