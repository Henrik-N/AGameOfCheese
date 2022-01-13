// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBall.h"


// Sets default values
AInteractableBall::AInteractableBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AInteractableBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBall::OnFocus_Implementation()
{
}

void AInteractableBall::OnFocusLost_Implementation()
{
}

void AInteractableBall::OnInteract_Implementation(AActor* Caller)
{
}

