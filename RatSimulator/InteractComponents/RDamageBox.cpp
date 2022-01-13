// Fill out your copyright notice in the Description page of Project Settings.


#include "RDamageBox.h"

#include "RatSimulator/RHealthComponent.h"

ARDamageBox::ARDamageBox()
{
	PrimaryActorTick.bCanEverTick = true;
}

//Deals damage to all actors inside the box in intervals
void ARDamageBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (OverlappedActors.Num() == 0)
		return;

	if (DamageTimeElapsed > TimeBetweenDamage)
	{
		DamageTimeElapsed = 0.f;
		for (AActor* Actor : OverlappedActors)
		{
			URHealthComponent* HP = Actor->FindComponentByClass<URHealthComponent>();
			if (HP)
			{
				HP->TakeDamage(DamageToDeal, GetOwner());
			}
		}
	}
	DamageTimeElapsed += DeltaSeconds;
}

//Bind overlaps
void ARDamageBox::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARDamageBox::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ARDamageBox::EndOverlap);
}

void ARDamageBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	URHealthComponent* HealthComponent = OtherActor->FindComponentByClass<URHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageToDeal, GetOwner());
		OverlappedActors.Add(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("OverlappedActor %s"), *OtherActor->GetName());
	}
}

void ARDamageBox::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActors.Empty();
}

void ARDamageBox::EmptyOverlappedActors()
{
	OverlappedActors.Empty();
}
