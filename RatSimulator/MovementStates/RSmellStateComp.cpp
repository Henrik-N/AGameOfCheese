#include "RSmellStateComp.h"

#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/DataAssets/RDA_CharacterMovementSettings.h"



URSmellStateComp::URSmellStateComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// timeline
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timelinecomp"));
}

void URSmellStateComp::TimelineTick_Implementation(float Alpha)
{
	
}

void URSmellStateComp::BeginPlay()
{
	Super::BeginPlay();

	RCharacter = Cast<ARCharacter>(GetOwner());
	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(RCharacter->GetMovementComponent());

	RCharacterCamera = RCharacter->FindComponentByClass<UCameraComponent>();
	
	// timeline
	CameraZOffsetTimelineFloat.BindDynamic(this, &URSmellStateComp::CameraZOffsetTimeline);
	TimelineComponent->AddInterpFloat(RCharacterMovementComponent->DataSettings->SmellCameraFloatCurveAlpha, CameraZOffsetTimelineFloat);
}

void URSmellStateComp::CameraZOffsetTimeline(float Alpha)
{	
	TimelineTick(Alpha);

	const auto DataSettings = RCharacterMovementComponent->DataSettings;

	const float CameraDefaultZ = RCharacter->DefaultCameraZOffset;
	const float CameraRaiseZ = DataSettings->CameraSmellZOffset;

	CameraZOffsetIntent = FMath::Lerp(CameraDefaultZ, CameraRaiseZ, Alpha); // camera is actually moved in RCharacterMovementComponent
}


void URSmellStateComp::OnEnableState_Implementation()
{
	RCharacterMovementComponent->SetMovementInputEnabled(false);

	TimelineComponent->Play();
}

void URSmellStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	RCharacterMovementComponent->SetMovementInputEnabled(true);

	TimelineComponent->Reverse();
}

