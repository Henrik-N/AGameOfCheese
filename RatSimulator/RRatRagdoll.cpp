// Fill out your copyright notice in the Description page of Project Settings.


#include "RRatRagdoll.h"


// Sets default values
ARRatRagdoll::ARRatRagdoll()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ARRatRagdoll::BeginPlay()
{
	Super::BeginPlay();
}



