#include "RVaultingComponent.h"

#include "RCharacter.h"
#include "RCharacterMovementComponent.h"
//#include "VaultPreviewComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "RMacros.h"

URVaultingComponent::URVaultingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URVaultingComponent::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent = GetOwner()->FindComponentByClass<UCapsuleComponent>();
	ensure(CapsuleComponent != nullptr);
	
	//PreviewComp = GetOwner()->FindComponentByClass<UVaultPreviewComponent>();
	//ensure(PreviewComp != nullptr);

	CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	ensure(CameraComponent != nullptr);

	MovementComponent = GetOwner()->FindComponentByClass<URCharacterMovementComponent>();
	ensure(MovementComponent != nullptr);
}


// Called every frame
void URVaultingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bHoldingForward = MovementComponent->GetCachedInputAxis().X > 0.5;
	
	if(!bIsVaulting && DataSettings->bAutoVaultInAir && bHoldingForward && MovementComponent->IsFalling())
	{
		CheckForAutoVault();
	}
	/*else
	{
		if(bInsideOverrideArea)
			PreviewComp->ActivatePreview(OverrideDestination->GetComponentLocation());
		else if(VaultPossible())
			PreviewComp->ActivatePreview(EndDestination);
		else
			PreviewComp->DisablePreview();
	}*/
}


bool URVaultingComponent::TryVault(bool bHasToSeeDestination)
{
	if(bIsVaulting)
		return false;

	/*if(bInsideOverrideArea && CheckIfInView(OverrideDestination->GetComponentLocation())) //always has to see the override to avoid confusion
	{
		EndDestination = OverrideDestination->GetComponentLocation();
		StartVault();
		return true;
	}
	else */if(VaultPossible(bHasToSeeDestination))
	{
		StartVault();
		OnStartVault.Broadcast();
		return true;
	}
	
	return false;
}

void URVaultingComponent::StartVault()
{
	//if(PreviewComp)
	//	PreviewComp->DisablePreview();
	
	bIsVaulting = true;
}

void URVaultingComponent::EndVault()
{
	bIsVaulting = false;
	OnEndVault.Broadcast();
}



bool URVaultingComponent::CheckIfInView(FVector Destination) const
{
	const FVector DirectionTowardsEnd = (Destination - CapsuleComponent->GetComponentLocation()).GetSafeNormal();
	const float Dot = FVector::DotProduct(DirectionTowardsEnd, CameraComponent->GetForwardVector()); //Dot product determines how centered the destination is to our view
	return Dot > DataSettings->LookAtTreshold;
}



void URVaultingComponent::CheckForAutoVault()
{
	FVector StartPos = CapsuleComponent->GetComponentLocation();
	FVector EndPos = StartPos + CapsuleComponent->GetForwardVector() * DataSettings->VaultRange;
	
	FHitResult Hit;
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(GetOwner()); //make sure we dont hit ourselves

	if(GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, DataSettings->TraceChannel, Params))
	{
		TryVault(false);
	}
}



bool URVaultingComponent::VaultPossible(bool bHasToSeeDestination)
{
	FVector StartPos = CapsuleComponent->GetComponentLocation();
	FVector EndPos = StartPos + (GetOwner()->GetActorUpVector() * DataSettings->MaxVaultHeight);
	float CurrentVaultHeight = DataSettings->MaxVaultHeight; //This is needed if we hit the roof
	
	FHitResult Hit;
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(GetOwner()); //make sure we dont hit ourselves

	//saving these two since they are used multiple times
	FQuat Rot = CapsuleComponent->GetComponentRotation().Quaternion();
	FCollisionShape ColShape = CapsuleComponent->GetCollisionShape();
	
	//First trace goes up
	if(GetWorld()->SweepSingleByChannel(Hit, StartPos, EndPos, Rot, DataSettings->TraceChannel, ColShape, Params, FCollisionResponseParams::DefaultResponseParam))
	{
		CurrentVaultHeight = Hit.Distance;

		float SqrDistance = FVector::DistSquared(Hit.Location, StartPos);
		if(SqrDistance < DataSettings->MinVaultHeight*DataSettings->MinVaultHeight)
		{
			//TODO: Debug
			if(bFireDebugSweepEvents)
				OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, false);
			return false;
		}
		//Can't end here since we want to see if there is a lower position that is still valid

		//TODO:DEBUG
		if(bFireDebugSweepEvents)
			OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, true);

		StartPos = Hit.Location- GetOwner()->GetActorUpVector() * DataSettings->RoofTolerance; //This is the position of the shape when the sweep hit
		EndPos = StartPos + GetOwner()->GetActorForwardVector() * DataSettings->VaultRange; //could technically be broken out, but easier to read this way
		
		Hit.Reset();
	}
	else
	{
		//TODO:DEBUG
		if(bFireDebugSweepEvents)
			OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, true);
		
		StartPos = EndPos;
		EndPos = StartPos + GetOwner()->GetActorForwardVector() * DataSettings->VaultRange;
	}

	MidPoint = StartPos;
	
	//Second trace goes forward from whichever up position we achieved
	if(GetWorld()->SweepSingleByChannel(Hit, StartPos, EndPos, Rot, DataSettings->TraceChannel, ColShape, Params, FCollisionResponseParams::DefaultResponseParam))
	{
		//TODO:DEBUG
			if(bFireDebugSweepEvents)
				OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, false);
		
		return false; //if we hit anything its game over
	}

		//TODO:DEBUG
			if(bFireDebugSweepEvents)
				OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, true);

	StartPos = EndPos;
	float DownWardsMaxDistance = CurrentVaultHeight - DataSettings->MinVaultHeight;
	EndPos = StartPos + (-GetOwner()->GetActorUpVector() * DownWardsMaxDistance);

	//During the last part we also need to trace with a line as well to see if we will actually be standing properly on ground 
	if(!GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, DataSettings->TraceChannel, Params, FCollisionResponseParams::DefaultResponseParam))
	{
		//TODO:DEBUG
		if(bFireDebugSweepEvents)
			OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, false);
		
		return false;
	}
	//Downward sweep from forward
	if(!GetWorld()->SweepSingleByChannel(Hit, StartPos, EndPos, Rot, DataSettings->TraceChannel, ColShape, Params, FCollisionResponseParams::DefaultResponseParam))
	{
		//TODO:DEBUG
			if(bFireDebugSweepEvents)
				OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, false);
		
		return false;
	}
	
	//TODO:DEBUG
	if(bFireDebugSweepEvents)
		OnDebugSweep.Broadcast(Hit, StartPos, EndPos, Rot, true);
	
	EndDestination = Hit.Location + GetOwner()->GetActorUpVector() * 20.f; //Quick fix to stop character from getting stuck in the ground when vaulting
	
	OnVaultPossible.Broadcast(Hit, StartPos, EndPos, Rot, true);

	if(bHasToSeeDestination)
	{
		return CheckIfInView(EndDestination);
	}
	
	return true;
}

/*void URVaultingComponent::StartOverride(const USceneComponent* NewOverrideDestination)
{
	OverrideDestination = NewOverrideDestination;
	bInsideOverrideArea = true;
}

void URVaultingComponent::EndOverride(const USceneComponent* OldOverrideDestination)
{
	//This check is here in case there are two overlapping override areas
	if(OldOverrideDestination == OverrideDestination)
	{
		bInsideOverrideArea = false;
	}
}
*/

