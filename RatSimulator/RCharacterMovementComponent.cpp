#include "RCharacterMovementComponent.h"


#include "DrawDebugHelpers.h"
#include "RCharacter.h"
#include "RMacros.h"
#include "RVaultingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/RDA_CharacterMovementSettings.h"
#include "MovementStates/RClimbableComponent.h"
#include "MovementStates/RCrouchStateComp.h"
#include "MovementStates/RWalkStateComp.h"
#include "Misc/Optional.h"

URCharacterMovementComponent::URCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.SetTickFunctionEnable(true);
}

void URCharacterMovementComponent::InitBpClasses()
{
	WalkStateComp = InitScriptInterface(WalkState1, DataSettings->WalkState1Component);
	// RunStateComp = InitScriptInterface(RunState1, DataSettings->RunState1Component);
	CrouchStateComp = InitScriptInterface(CrouchState1, DataSettings->CrouchState1Component);
	SlideStateComp = InitScriptInterface(SlideState1, DataSettings->SlideState1Component);
	VaultStateComp = InitScriptInterface(VaultState1, DataSettings->VaultState1Component);
	ClimbStateComp = InitScriptInterface(ClimbState1, DataSettings->ClimbState1Component);
	SmellStateComp = InitScriptInterface(SmellState1, DataSettings->SmellState1Component);
}

template <class T>
T* URCharacterMovementComponent::InitScriptInterface(TScriptInterface<IRStateInterface>& Interface,
                                                     TSubclassOf<T> Object)
{
	const auto Component = Cast<T>(GetOwner()->AddComponentByClass(Object, false, FTransform::Identity, false));
	Interface.SetObject(Cast<UObject>(Component));
	Interface.SetInterface(Cast<IRStateInterface>(Component));
	return Component;
}

void URCharacterMovementComponent::ChangeState(const TScriptInterface<IRStateInterface>& InNewState)
{
	if (CurrentStateComponent == InNewState)
	{
		return;
	}

	IRStateInterface::Execute_OnDisableState(CurrentStateComponent.GetObject(), InNewState);

	UE_LOG(LogTemp, Warning, TEXT("Changing state: %s \t--> %s"), *CurrentStateComponent.GetObject()->GetName(),
	       *InNewState.GetObject()->GetName());
	CurrentStateComponent = InNewState;
	IRStateInterface::Execute_OnEnableState(CurrentStateComponent.GetObject());
}

TScriptInterface<IRStateInterface> URCharacterMovementComponent::GetCurrentState() const
{
	return CurrentStateComponent;
}

float URCharacterMovementComponent::GetMovementPercent() const
{
	return Velocity.Size() / GetMaxSpeed();
}

float URCharacterMovementComponent::RCalculateDirectionClimbing(const FVector& RVelocity,
	const FRotator& BaseRotation) const
{
	if (!RVelocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = (FVector(0, RVelocity.Y, RVelocity.Z)).GetSafeNormal();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
			{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

void URCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();


	InitBpClasses();
	CurrentStateComponent = WalkState1;

	RCharacter = Cast<ARCharacter>(GetOwner());

	
	DefaultGravityScale = GravityScale;

	const auto VaultingComponent = Cast<ARCharacter>(GetOwner())->FindComponentByClass<URVaultingComponent>();


	RCharacter->OnActorHit.AddDynamic(this, &URCharacterMovementComponent::OnPawnHit);
	
	VaultingComponent->OnStartVault.AddDynamic(this, &URCharacterMovementComponent::OnStartVault); // "OnStartVault");
	VaultingComponent->OnEndVault.AddDynamic(this, &URCharacterMovementComponent::OnEndVault);

	//// SET


	const auto CapsuleComponent = Cast<ARCharacter>(GetOwner())->GetCapsuleComponent();
	

	if (RCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OWNING CHARACTER NULL"))
	}

	StandingCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();

	StandingMeshHeight = RCharacter->GetMesh()->GetRelativeLocation().Z;


	RCharacterCameraComp = RCharacter->FindComponentByClass<UCameraComponent>();
}


void URCharacterMovementComponent::SetMovementInputEnabled(bool bInInputEnabled)
{
	bInputEnabled = bInInputEnabled;
}

FVector2D URCharacterMovementComponent::GetCachedInputAxis() const
{
	return InputAxis;
}

void URCharacterMovementComponent::MoveForward(float Value)
{
	InputAxis.X = Value;
	if (!bInputEnabled || CurrentStateComponent == SlideState1) return;
	
	if (Value != 0.0f && CurrentStateComponent != ClimbState1)
	{
		if (Value > 0.f)
		{
			Value *= DataSettings->ForwardMovementFraction;
		}
		else
		{
			Value *= DataSettings->BackwardsMovementFraction;
		}

		RCharacter->AddMovementInput(RCharacter->GetActorForwardVector(), Value);
	}
}

void URCharacterMovementComponent::MoveRight(float Value)
{
	InputAxis.Y = Value;
	if (!bInputEnabled || CurrentStateComponent == SlideState1) return;

	if (Value != 0.0f && CurrentStateComponent != ClimbState1)
	{
		Value *= DataSettings->SidewaysMovementFraction;
		
		RCharacter->AddMovementInput(RCharacter->GetActorRightVector(), Value);
	}
}


void URCharacterMovementComponent::SlideClampVelocity()
{
	// clamp velocity
	if (Velocity.Size() > DataSettings->MaxSlideSpeed)
	{
		Velocity = Velocity.GetSafeNormal() * DataSettings->MaxSlideSpeed;
	}
}

void URCharacterMovementComponent::ChangeStateIfTooSlow()
{
	if (Velocity.Size() < DataSettings->CrouchMovementSpeed)
	{
		ResolveMovementState();
	}
}

FVector URCharacterMovementComponent::CalculateFloorSlopeInfluence() const
{
	const FVector FloorNormal = CurrentFloor.HitResult.Normal;


	if (FloorNormal.Equals(FVector::ZAxisVector, 0.0001))
	{
		return FVector::ZeroVector;
	}

	// todo: Same as vector plane project right?
	const FVector Cross = FVector::CrossProduct(FloorNormal, FVector::ZAxisVector);
	const FVector VectorAlongSurface = FVector::CrossProduct(FloorNormal, Cross);

	return VectorAlongSurface.GetSafeNormal();
}

bool URCharacterMovementComponent::CanStandUp() const
{
	FHitResult HitResult;
	const auto CapsuleComponent = Cast<ARCharacter>(GetOwner())->GetCapsuleComponent();

	const float CurrentCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	// may be scaled since it will scale down while crouching

	const FVector GroundLocation = CapsuleComponent->GetComponentLocation() + (-FVector::ZAxisVector *
		CurrentCapsuleHalfHeight);
	const FVector TopOfCapsuleLocation = GroundLocation + (FVector::ZAxisVector * StandingCapsuleHalfHeight * 2);


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bFullCapsuleWouldNotFit = GetWorld()->LineTraceSingleByChannel(HitResult, GroundLocation, TopOfCapsuleLocation,
	                                                                    ECC_Visibility, QueryParams);

	return !bFullCapsuleWouldNotFit;
}

void URCharacterMovementComponent::OnSprintKeyDown()
{
	bSprintKeyDown = true;
	if (CurrentStateComponent != VaultState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnSprintKeyUp()
{
	bSprintKeyDown = false;
	if (CurrentStateComponent != VaultState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnCrouchKeyDown()
{
	bCrouchKeyDown = true;
	if (CurrentStateComponent != VaultState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnCrouchKeyUp()
{
	bCrouchKeyDown = false;
	if (CurrentStateComponent != VaultState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnSmellKeyDown()
{
	bSmellKeyDown = true;
	if (CurrentStateComponent != VaultState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnSmellKeyUp()
{
	bSmellKeyDown = false;
	if (CurrentStateComponent != VaultState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnChargeJumpKeyUp()
{
	bJumpChargeKeyDown = false;
	if (CurrentStateComponent != VaultState1 && CurrentStateComponent != ClimbState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::OnChargeJumpKeyDown()
{
	bJumpChargeKeyDown = true;
	if (CurrentStateComponent != VaultState1 && CurrentStateComponent != ClimbState1)
	{
		ResolveMovementState();
	}
}

void URCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float CrouchZCamOffset = CrouchStateComp->CameraZOffsetIntent;
	const float SmellZCamOffset = SmellStateComp->CameraZOffsetIntent;

	const float NewCameraZOffset = RCharacter->DefaultCameraZOffset + CrouchZCamOffset + SmellZCamOffset;

	FVector CameraRelativeLoc = RCharacterCameraComp->GetRelativeLocation();
	CameraRelativeLoc.Z = NewCameraZOffset;
	RCharacterCameraComp->SetRelativeLocation(CameraRelativeLoc);
}

TOptional<FHitResult> URCharacterMovementComponent::RaycastFromChar(const FVector& Direction, float RayDistance) const
{
	const FVector StartLoc = RCharacter->GetActorLocation();
	const FVector EndLoc = StartLoc + Direction * RayDistance;

	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(RCharacter);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLoc, EndLoc, ECC_Visibility, QueryParams))
	{
		return TOptional<FHitResult>(OutHit);
	}

	return TOptional<FHitResult>();
}

void URCharacterMovementComponent::OnPawnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (CurrentStateComponent != VaultState1 && IsFalling() && OtherActor->FindComponentByClass<URClimbableComponent>() != nullptr)
	{
		ChangeState(ClimbState1);
	}
}

bool URCharacterMovementComponent::CanClimb()
{
	const auto OptionalOutHit = RaycastFromChar(RCharacter->GetActorForwardVector(), 100.f);

	if (!OptionalOutHit.IsSet())
	{
		return false;
	}

	const auto OutHit = OptionalOutHit.GetValue();

	if (!OutHit.bBlockingHit)
	{
		// Can climb? Ray didn't hit anything.
		return false;
	}

	bool bCanClimb;
	if (OutHit.Actor->FindComponentByClass<URClimbableComponent>() != nullptr)
	{
		// Can climb? Yes, found a climbable component
		bCanClimb = true;
		UE_LOG(LogTemp, Warning, TEXT("CAN CLIMB!"))
	}
	else
	{
		// Can climb? No, didn't find climbable component, can NOT climb.
		bCanClimb = false;
		UE_LOG(LogTemp, Warning, TEXT("CAN'T CLIMB!"))
	}
	return bCanClimb;
}

void URCharacterMovementComponent::OnStartVault()
{
	// ChangeState(VaultingState);
	ChangeState(VaultState1);
}

void URCharacterMovementComponent::OnEndVault()
{
	ResolveMovementState();
}

void URCharacterMovementComponent::ResolveMovementState()
{
	if (CurrentStateComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current state is NULL"))
		return;
	}


	if (CurrentStateComponent == ClimbState1 && bJumpChargeKeyDown) // released jump while on the wall
	{
		// disable state will launch the character
		//ChangeState(WalkState1);
		//ClimbStateComp->Leap();
		return;
	}


	// todo Fix so that if you hold down the crouch key and press smell, jumpcharge is interupted and instead start smelling
	// todo Can be done in the DisableState by checking the upcoming state. If it's smelling, don't jump.

	const bool bCanStand = CanStandUp();

	if (bSmellKeyDown && bCanStand && !IsFalling())
	{
		if (CurrentStateComponent != CrouchState1)
		{
			ChangeState(SmellState1);
		}

		return;
	}

	if (bCrouchKeyDown || !bCanStand)
	{
		const float CurrentSpeed = this->Velocity.Size();

		if (CurrentSpeed > DataSettings->WalkSpeed /* Movement speed is higher than walking speed */)
		{
			// Holding crouch and faster than walking speed
			ChangeState(SlideState1);
		}
		else
		{
			// Holding crouch and at walking speed
			ChangeState(CrouchState1);
		}

		return;
	}

	if (bJumpChargeKeyDown)
	{
		ChangeState(CrouchState1);

		return;
	}

	// if (bSprintKeyDown)
	// {
	// 	ChangeState(RunState1);
	//
	// 	return;
	// }

	ChangeState(WalkState1);
}

// todo? Wallrun
// void URCharacterMovementComponent::CheckForWallrunTick(float DeltaTime)
// {
// 	const FVector CurrentActorLocation = OwningCharacter->GetActorLocation();
// 	const FVector ForwardVec = OwningCharacter->GetActorForwardVector();
// 	const FVector RightVec = OwningCharacter->GetActorRightVector();
//
// 	const FVector RightEndPoint = CurrentActorLocation + ForwardVec * -35.f + RightVec * 75.f;
// 	const FVector LeftEndPoint = CurrentActorLocation + ForwardVec * -35.f + RightVec * -75.f;
//
//
// 	// raycast for walls
// 	FHitResult LeftOutHit;
// 	FHitResult RightOutHit;
// 	FCollisionQueryParams QueryParams;
// 	QueryParams.AddIgnoredActor(GetOwner());
//
// 	GetWorld()->LineTraceSingleByChannel(LeftOutHit, CurrentActorLocation, LeftEndPoint, ECC_Visibility, QueryParams);
// 	GetWorld()->LineTraceSingleByChannel(RightOutHit, CurrentActorLocation, RightEndPoint, ECC_Visibility, QueryParams);
//
// 	FVector ForwardAlongWallDir;
// 	FVector WallNormal;
// 	FHitResult HitResult;
//
// 	if (LeftOutHit.bBlockingHit)
// 	{
// 		if (LeftOutHit.Normal.Z > -0.2 && LeftOutHit.Normal.Z < 0.2) // wall is about 90 degrees
// 		{
// 			ForwardAlongWallDir = FVector::CrossProduct(LeftOutHit.Normal, FVector::ZAxisVector);
// 			UE_LOG(LogTemp, Warning, TEXT("ForwardAlongWallDir: %f, %f, %f"), ForwardAlongWallDir.X,
// 			       ForwardAlongWallDir.Y, ForwardAlongWallDir.Z);
// 			WallNormal = LeftOutHit.Normal;
// 			HitResult = LeftOutHit;
// 		}
// 	}
// 	else if (RightOutHit.bBlockingHit)
// 	{
// 		if (RightOutHit.Normal.Z > -0.2 && RightOutHit.Normal.Z < 0.2) // wall is about 90 degrees
// 		{
// 			ForwardAlongWallDir = FVector::CrossProduct(FVector::ZAxisVector, RightOutHit.Normal);
// 			UE_LOG(LogTemp, Warning, TEXT("ForwardAlongWallDir: %f, %f, %f"), ForwardAlongWallDir.X,
// 			       ForwardAlongWallDir.Y, ForwardAlongWallDir.Z);
//
// 			WallNormal = RightOutHit.Normal;
// 			HitResult = RightOutHit;
// 		}
// 	}
// 	else
// 	{
// 		return;
// 	}
//
// 	if (IsFalling())
// 	{
// 		// launch towards wall
// 		const auto LaunchVector = WallNormal * (CurrentActorLocation - WallNormal).Size();
// 		OwningCharacter->LaunchCharacter(LaunchVector, false, false);
//
// 		const float WallrunSpeed = 850.f; // * DeltaTime
// 		// launch along wall
// 		OwningCharacter->LaunchCharacter(ForwardAlongWallDir * WallrunSpeed, true, true);
// 	}
// }
