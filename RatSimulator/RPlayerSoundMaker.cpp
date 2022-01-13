// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerSoundMaker.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void URPlayerSoundMaker::SpawnSound(EPhysicalSurface Surface)
{
	//Use surface as key to find matching sound in map Sounds
	UGameplayStatics::SpawnSoundAtLocation(this, Sounds[Surface], GetOwner()->GetActorLocation());
}

void URPlayerSoundMaker::PlayFootstep()
{
	const int LineTraceLength = 200; //Arbitrary big value since running animation wont play unless on ground already.
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + (-(GetOwner()->GetActorUpVector()) * LineTraceLength);

	FHitResult Hit;
	
	if(UKismetSystemLibrary::LineTraceSingle(
		this,
		Start, End,
		TraceChannel,
		false,
		TArray<AActor*>{},
		EDrawDebugTrace::None,
		Hit,
		true
		))
	{
		const EPhysicalSurface Surf = UGameplayStatics::GetSurfaceType(Hit);
		if(Surf)
		{
			SpawnSound(Surf);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO ESURFACE FOUND FOR FOOTSTEP SOUNDS"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GROUND FOUND FOR FOOTSTEP SOUNDS"));
	}
}

