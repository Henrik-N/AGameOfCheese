#include "RCrouchStateComp.h"

#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/DataAssets/RDA_CharacterMovementSettings.h"

URCrouchStateComp::URCrouchStateComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timelinecomp"));
}

void URCrouchStateComp::BeginPlay()
{
	Super::BeginPlay();
	RCharacter = Cast<ARCharacter>(GetOwner());
	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(RCharacter->GetMovementComponent());

	RCharacterCamera = RCharacter->FindComponentByClass<UCameraComponent>();

	const auto DataSettings = RCharacterMovementComponent->DataSettings;
	
	// Camera local vertical movement timeline
	CameraZOffsetTimelineFloat.BindDynamic(this, &URCrouchStateComp::CameraZoffsetTimeline);
	TimelineComponent->AddInterpFloat(DataSettings->CrouchCameraFloatCurveAlpha, CameraZOffsetTimelineFloat); // bind curve to function


	// Jump charge timeline
	JumpChargeTimelineFloat.BindDynamic(this, &URCrouchStateComp::JumpChargeTimeline);
	TimelineComponent->AddInterpFloat(DataSettings->JumpChargeCurve, JumpChargeTimelineFloat); // bind curve to function

	JumpChargeUpgradeInfluenceTimelineFloat.BindDynamic(this, &URCrouchStateComp::JumpChargeUpgradeInfluenceTimeline);
	TimelineComponent->AddInterpFloat(DataSettings->JumpChargeUpgradeInfluenceOverJumpChargeCurve, JumpChargeUpgradeInfluenceTimelineFloat);
}

void URCrouchStateComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (RCharacterMovementComponent->IsFalling())
	{
		CoyoteTimer += DeltaTime;
	}
	else // grounded
	{
		bHasJumpedSinceLastGrounded = false;
		CoyoteTimer = 0.f;
	}
}

bool URCrouchStateComp::CanJump() const
{
	if (CoyoteTimer < RCharacterMovementComponent->DataSettings->CoyoteTimeGracePeriod)
	{
		if (!bHasJumpedSinceLastGrounded)
		{
			return true;
		}
	}
	
	return false;
}

void URCrouchStateComp::CameraZoffsetTimeline(float Alpha)
{
	const auto DataSettings = RCharacterMovementComponent->DataSettings;

	const float CameraDefaultZ = RCharacter->DefaultCameraZOffset;
	const float CrouchCameraZ = DataSettings->CameraCrouchZOffset;
	
	CameraZOffsetIntent = FMath::Lerp(CrouchCameraZ, CameraDefaultZ, Alpha); // camera is actually moved in RCharacterMovementComponent
}

void URCrouchStateComp::JumpChargeUpgradeInfluenceTimeline(float Output)
{
	JumpChargeUpgradeInfluence = Output;
}

void URCrouchStateComp::JumpChargeTimeline(float Output)
{
	JumpCharge = Output;

	const int ConsumedMethCount = RCharacter->ConsumedMeth;
	const float IncreasePerUpgrade = RCharacterMovementComponent->DataSettings->JumpChargeIncreasePerJumpChargeUpgrade;

	const float AdditionalJumpChargeFromUpgrade = ConsumedMethCount * (JumpChargeUpgradeInfluence * IncreasePerUpgrade);
	
	JumpCharge = Output + AdditionalJumpChargeFromUpgrade;
}

void URCrouchStateComp::OnEnableState_Implementation()
{
	TimelineComponent->Play();
}

void URCrouchStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	if (CanJump())
	{
		bHasJumpedSinceLastGrounded = true;
		const FVector LaunchVelocity = FVector(0.f, 0.f, JumpCharge);
		RCharacter->LaunchCharacter(LaunchVelocity, false, true);
	}
	TimelineComponent->Reverse();	
}
