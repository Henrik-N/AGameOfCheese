#include "RWalkStateComp.h"

#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/DataAssets/RDA_CharacterMovementSettings.h"

URWalkStateComp::URWalkStateComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URWalkStateComp::BeginPlay()
{
	Super::BeginPlay();

	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(Cast<ARCharacter>(GetOwner())->GetMovementComponent());
}

void URWalkStateComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// if (bTick)
	// {
		// RCharacterMovementComponent->InputAxis.X > 
	// }
}

void URWalkStateComp::OnEnableState_Implementation()
{
	RCharacterMovementComponent->MaxWalkSpeed = RCharacterMovementComponent->DataSettings->WalkSpeed;
}

void URWalkStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	
}
