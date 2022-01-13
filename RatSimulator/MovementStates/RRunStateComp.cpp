#include "RRunStateComp.h"

#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/DataAssets/RDA_CharacterMovementSettings.h"

URRunStateComp::URRunStateComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URRunStateComp::BeginPlay()
{
	Super::BeginPlay();

	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(Cast<ARCharacter>(GetOwner())->GetMovementComponent());
}

void URRunStateComp::OnEnableState_Implementation()
{
	//RCharacterMovementComponent->MaxWalkSpeed = RCharacterMovementComponent->DataSettings->SprintingSpeed;
}

void URRunStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	
}

