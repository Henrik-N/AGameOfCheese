// Fill out your copyright notice in the Description page of Project Settings.


#include "RInteractComponent.h"
#include "InteractComponents/InteractInterface.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
URInteractComponent::URInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	FColParam.AddIgnoredActor(GetOwner());
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
}


// Called every frame
void URInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector Direction = Camera->GetForwardVector();
	const FVector Origin = GetOwner()->GetActorLocation();

	if (bDebugDraw)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Direction * InteractRange, FLinearColor::Blue);
	}

	TraceForward(Origin, Direction);
}

void URInteractComponent::TraceForward(FVector Origin, FVector Direction)
{
	FHitResult Hit;
	//GameTraceChannel1 is a custom interact trace collision
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Origin, Origin + Direction * InteractRange,
                                                     ECC_GameTraceChannel1,
                                                     FColParam);
	if (bHit)
	{
		AActor* Interactable = Hit.GetActor();

		if (Interactable)
		{
			if (Interactable != FocusedActor)
			{
				if (FocusedActor)
				{
					IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
					if (Interface)
					{
						Interface->Execute_OnFocusLost(FocusedActor);
					}
				}
				IInteractInterface* Interface = Cast<IInteractInterface>(Interactable);
				if (Interface)
				{
					// if (bWantToInteract)
					// {
					// 	Interface->Execute_OnInteract(FocusedActor, GetOwner());
					// }
					Interface->Execute_OnFocus(Interactable);
				}
			}
			FocusedActor = Interactable;
		}
	}
	else
	{
		if (FocusedActor)
		{
			IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
			if (Interface)
			{
				Interface->Execute_OnFocusLost(FocusedActor);
			}
		}
		FocusedActor = nullptr;
	}
}

void URInteractComponent::InteractPressed()
{
	TraceForward(GetOwner()->GetActorLocation(),Camera->GetForwardVector());
	if (FocusedActor && bShouldInteract)
	{
		IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
		if (Interface)
		{
			Interface->Execute_OnInteract(FocusedActor, GetOwner());
		}
	}
}

