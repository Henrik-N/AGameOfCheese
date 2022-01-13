#include "RVaultStateComp.h"

#include "RatSimulator/RCharacter.h"
#include "RatSimulator/RCharacterMovementComponent.h"
#include "RatSimulator/RVaultingComponent.h"
#include "RatSimulator/DataAssets/RDA_VaultingSettings.h"

URVaultStateComp::URVaultStateComp()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;
}

void URVaultStateComp::BeginPlay()
{
	Super::BeginPlay();

	ARCharacter* Character= Cast<ARCharacter>(GetOwner());
	RCharacterMovementComponent = Cast<URCharacterMovementComponent>(Character->GetCharacterMovement());
	VaultingComponent = Character->VaultingComponent;
	DataSettings = VaultingComponent->DataSettings;
}

void URVaultStateComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveTowardsDestination(DeltaTime);
}

void URVaultStateComp::OnEnableState_Implementation()
{
	
	VaultStartVelocity = RCharacterMovementComponent->Velocity;
	VaultStartVelocity.Z = 0; //we want the movement without the jump
	VaultStartPos = GetOwner()->GetActorLocation();
	VaultStartSpeedPercent = RCharacterMovementComponent->GetMovementPercent();
	
	RCharacterMovementComponent->SetMovementInputEnabled(false);
	RCharacterMovementComponent->GravityScale = 0.f;
	RCharacterMovementComponent->Velocity = FVector::ZeroVector;
	
	LerpTimer = 0;
	SetComponentTickEnabled(true);
}

void URVaultStateComp::OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState)
{
	SetComponentTickEnabled(false);
	RCharacterMovementComponent->GravityScale = RCharacterMovementComponent->DefaultGravityScale;
	RCharacterMovementComponent->SetMovementInputEnabled(true);

	const FVector ToEndPos = VaultingComponent->GetEndDestination() - VaultStartPos;
	const float Dot = FVector::DotProduct(VaultStartVelocity.GetSafeNormal(), ToEndPos.GetSafeNormal());

	const float T = InverseLerp(DataSettings->LowerRangeDot, DataSettings->UpperRangeDot, Dot);
	const float EndVelocityMultiplier = FMath::Lerp(DataSettings->MinEndVelocityMultiplier, DataSettings->MaxEndVelocityMultiplier, T);

	RCharacterMovementComponent->Velocity = VaultStartVelocity * EndVelocityMultiplier;
}

void URVaultStateComp::MoveTowardsDestination(float DeltaTime)
{
	const FTransform ActorTransform = GetOwner()->GetTransform();
	const float VaultSpeed = FMath::Lerp(DataSettings->MinVaultSpeed, DataSettings->MaxVaultSpeed, VaultStartSpeedPercent);
	LerpTimer += VaultSpeed * DeltaTime;
	LerpTimer = FMath::Min(LerpTimer, 1.0f);
	
	FVector TempMidPoint = VaultingComponent->GetMidPoint();
	if(DataSettings->BezierHeightMultiplier > 1.0f)
	{
		const float MidPointRelativeZ = TempMidPoint.Z - VaultStartPos.Z;
		TempMidPoint.Z = MidPointRelativeZ * DataSettings->BezierHeightMultiplier + VaultStartPos.Z;
	}
	const FVector CurrentPos = Bezier3Points(VaultStartPos, TempMidPoint, VaultingComponent->GetEndDestination(), LerpTimer);
	
	GetOwner()->SetActorLocation(CurrentPos);
	
	if((CurrentPos-VaultingComponent->GetEndDestination()).IsNearlyZero(DataSettings->VaultTolerance))
	{
		VaultingComponent->EndVault();
	}
	
}

float URVaultStateComp::InverseLerp(float A, float B, float Value) const
{
	return (Value - A) / (B - A);
}

FVector URVaultStateComp::Bezier3Points(const FVector& A, const FVector& B, const FVector& C, float T) const
{
	const FVector D = FMath::Lerp(A, B, T);
	const FVector E = FMath::Lerp(B, C, T);
	return FMath::Lerp(D,E,T);
}
