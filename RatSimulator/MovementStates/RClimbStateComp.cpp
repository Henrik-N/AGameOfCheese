#include "RClimbStateComp.h"

#include "DrawDebugHelpers.h"
#include "RClimbableComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/DataAssets/RDA_CharacterMovementSettings.h"

URClimbStateComp::URClimbStateComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void URClimbStateComp::BeginPlay()
{
	Super::BeginPlay();
	RCharacter = Cast<ARCharacter>(GetOwner());
	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(RCharacter->GetMovementComponent());

	RCharacterCamera = RCharacter->FindComponentByClass<UCameraComponent>();

	RCharacter->OnActorHit.AddDynamic(this, &URClimbStateComp::OnPawnHit);
}

TOptional<FRSurfaceInfo3> URClimbStateComp::FindSurfaceFromDirectionFromActor(
	AActor* Actor, const FVector& DirectionFromActor)
{
	const float TraceDistance = 3000.f;
	const FVector ActorLocation = Actor->GetActorLocation();


	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Actor);

	FHitResult HitResult;
	Actor->GetWorld()->LineTraceSingleByChannel(HitResult, ActorLocation,
	                                            ActorLocation + DirectionFromActor * TraceDistance,
	                                            ECC_Visibility, CollisionQueryParams);

	FRSurfaceInfo3 SurfaceInfo;
	if (HitResult.bBlockingHit)
	{
		SurfaceInfo.SurfaceNormal = HitResult.Normal;
		SurfaceInfo.DistanceToSurface = HitResult.Distance;
	}
	else
	{
		return TOptional<FRSurfaceInfo3>();
	}

	return TOptional<FRSurfaceInfo3>(SurfaceInfo);
}


void URClimbStateComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	if (bTick) //todo: fix tick
	{
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		
		const bool bIsSurfaceKindOfHorizontal = ShouldStopClimbingSurface(ClimbSurfaceNormal);

		
		TimeSinceClimbNormalUpdated += DeltaTime;
		if (bIsSurfaceKindOfHorizontal || TimeSinceClimbNormalUpdated > 0.5f)
		{
			RCharacterMovementComponent->ChangeState(RCharacterMovementComponent->WalkState1);

			bTick = false;

			return;
		}

		FollowWall(DeltaTime);
	}
}

void URClimbStateComp::OnPawnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->FindComponentByClass<URClimbableComponent>() != nullptr)
	{
		ClimbSurfaceNormal = Hit.Normal;
		TimeSinceClimbNormalUpdated = 0.0f;
	}
}

bool URClimbStateComp::IsSurfaceVertical(const FVector& SurfaceNormal)
{
	const float NormalUpDirEquivalence = FVector::DotProduct(FVector::UpVector, SurfaceNormal);

	bool bSurfaceIsVertical;

	if (NormalUpDirEquivalence > -0.01 && NormalUpDirEquivalence < 0.01)
	{
		bSurfaceIsVertical = true;
	}
	else
	{
		bSurfaceIsVertical = false;
	}

	return bSurfaceIsVertical;
}

bool URClimbStateComp::ShouldStopClimbingSurface(const FVector& WallNormal)
{
	const float SimilarityToFloor = FVector::DotProduct(WallNormal, FVector::UpVector);

	if (SimilarityToFloor > 0.95f || SimilarityToFloor < -0.02f)
	{
		return true;
	}
	return false;
}

bool URClimbStateComp::IsFacingWall(const FVector& WallNormal)
{
	const auto PlanedSurfaceNormal = FVector::VectorPlaneProject(WallNormal, FVector::UpVector);
	const float DotFacingWall = FVector::DotProduct(RCharacterCamera->GetForwardVector(), -PlanedSurfaceNormal);

	if (DotFacingWall > 0.f)
	{
		return true;
	}

	return false;
}

void URClimbStateComp::FollowWall(float DeltaTime)
{
	const bool bWallIsVertical = IsSurfaceVertical(ClimbSurfaceNormal);

	FVector ActorForwardDir;
	FRotator NewRotation;

	if (!bWallIsVertical) // not vertical wall
	{
		RCharacterCamera->bUsePawnControlRotation = true;
		ActorForwardDir = GetOwner()->GetActorForwardVector();
		NewRotation = UKismetMathLibrary::MakeRotFromZX(ClimbSurfaceNormal, ActorForwardDir);
	}
	else
	{
		RCharacterCamera->bUsePawnControlRotation = true;

		ActorForwardDir = FVector::UpVector;

		NewRotation = UKismetMathLibrary::MakeRotFromZX(ClimbSurfaceNormal, ActorForwardDir);
	}

	const FVector GravityMove = -ClimbSurfaceNormal * RCharacterMovementComponent->DefaultGravityScale * 100.f;

	FHitResult OutHit;
	RCharacterMovementComponent->SafeMoveUpdatedComponent(GravityMove * DeltaTime, NewRotation, true, OutHit,
	                                                      ETeleportType::ResetPhysics);

	const FVector ForwardMove = RCharacterMovementComponent->InputAxis.X * RCharacter->GetActorForwardVector() *
		RCharacterMovementComponent->DataSettings->ForwardClimbSpeed;

	bFacingWall = IsFacingWall(ClimbSurfaceNormal);

	const float RightClimbSpeed = RCharacterMovementComponent->DataSettings->RightClimbSpeed;

	const FVector RightMove = RCharacterMovementComponent->InputAxis.Y * RCharacter->GetActorRightVector() *
		RightClimbSpeed;

	RCharacterMovementComponent->SafeMoveUpdatedComponent((ForwardMove + RightMove) * DeltaTime, NewRotation, false, OutHit,
	                                                      ETeleportType::ResetPhysics);

	// set velocity for use in animation blueprint
	RCharacterMovementComponent->Velocity = GravityMove + ForwardMove + RightMove;
}

void URClimbStateComp::SetDefaultGravityEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		RCharacterMovementComponent->GravityScale = RCharacterMovementComponent->DefaultGravityScale;
	}
	else
	{
		RCharacterMovementComponent->GravityScale = 0.f;
	}
}

void URClimbStateComp::StopVelocity()
{
	RCharacterMovementComponent->Velocity = FVector::ZeroVector;
}

void URClimbStateComp::ResetActorRotation()
{
	const FVector ForwardDir = RCharacter->GetActorForwardVector();
	const FVector ForwardX = FVector::VectorPlaneProject(ForwardDir, FVector::UpVector);
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromXZ(ForwardX, FVector::UpVector);

	RCharacter->SetActorRotation(Rotation, ETeleportType::ResetPhysics);
}

void URClimbStateComp::OnEnableState_Implementation()
{
	SetDefaultGravityEnabled(false);
	StopVelocity();
	bTick = true;
}

void URClimbStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	bTick = false;
	ResetActorRotation();
	StopVelocity();
	SetDefaultGravityEnabled(true);

	RCharacterCamera->bUsePawnControlRotation = true;
}

void URClimbStateComp::Leap()
{
	const FVector LaunchVelocity = ClimbSurfaceNormal * RCharacterMovementComponent->DataSettings->LeapForce;

	RCharacter->LaunchCharacter(LaunchVelocity, true, true); // todo: Do RCharacter->LaunchCharacter instead?
}

void URClimbStateComp::DebugDrawLeapPredict(float DeltaTime)
{
	FPredictProjectilePathParams Params;


	Params.LaunchVelocity = RCharacterCamera->GetForwardVector() * RCharacterMovementComponent->DataSettings->LeapForce;


	Params.StartLocation = RCharacter->GetActorLocation();
	Params.ProjectileRadius = RCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	Params.OverrideGravityZ = true;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(RCharacter);
	Params.ActorsToIgnore = ActorsToIgnore;

	Params.bTraceWithCollision = true;

	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;


	//Params.TraceChannel = ECC_Visibility;

	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(GetWorld(), Params, Result);
}
