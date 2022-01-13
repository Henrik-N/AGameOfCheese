#include "RSlideStateComp.h"

#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/DataAssets/RDA_CharacterMovementSettings.h"

URSlideStateComp::URSlideStateComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URSlideStateComp::BeginPlay()
{
	Super::BeginPlay();
	RCharacter = Cast<ARCharacter>(GetOwner());
	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(RCharacter->GetCharacterMovement());

	DefaultGroundFriction = RCharacterMovementComponent->GroundFriction;
	DefaultBrakingDeceleration = RCharacterMovementComponent->BrakingDecelerationWalking;
}

void URSlideStateComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URSlideStateComp::SetSlideVelocity()
{
	const FVector ForwardVec = RCharacter->GetActorForwardVector();
	const float MaxSlideSpeed = RCharacterMovementComponent->DataSettings->MaxSlideSpeed;
	RCharacterMovementComponent->Velocity = ForwardVec * MaxSlideSpeed;
}

void URSlideStateComp::SetGroundFrictionAndDeceleration()
{
	RCharacterMovementComponent->GroundFriction = 0.f;
	RCharacterMovementComponent->BrakingDecelerationWalking = 1000.f;
}

void URSlideStateComp::ResetGroundFrictionAndDeceleration()
{
	RCharacterMovementComponent->GroundFriction = DefaultGroundFriction;
	RCharacterMovementComponent->BrakingDecelerationWalking = DefaultBrakingDeceleration;
}

void URSlideStateComp::OnEnableState_Implementation()
{
	SetSlideVelocity();
	SetGroundFrictionAndDeceleration();
}

void URSlideStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	ResetGroundFrictionAndDeceleration();
}
