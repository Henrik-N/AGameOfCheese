// Fill out your copyright notice in the Description page of Project Settings.


#include "RHealthComponent.h"


// Sets default values for this component's properties
URHealthComponent::URHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DefaultHealth = 100.f;
	CurrentHealth = DefaultHealth;
}

//Regenerates health based on time but not if damage has been dealt recently
void URHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentHealth >= DefaultHealth)
		return;

	if (HealthTimer > HealthRegenRate && TimeSinceLastDamage > DamageTimer)
	{
		HealthTimer = 0.f;
		CurrentHealth += HealthToRegenerate;
		if (bShowDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Regnerating Health"));
		}
	}
	HealthTimer += DeltaTime;
	TimeSinceLastDamage += DeltaTime;
}

//Default damage implementation of just reducing health
void URHealthComponent::TakeDamage_Implementation(float Damage, AActor* Instigator)
{
	if (Damage <= 0)
		return;

	if (bShowDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Taking Damage for real"));
		UE_LOG(LogTemp, Log, TEXT("Health %f"), CurrentHealth);
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, DefaultHealth);
	TimeSinceLastDamage = 0.f;

	 if (CurrentHealth == 0)
	 {
	 	Die(Instigator);
	 }
}

//Method for adding health to the player
void URHealthComponent::AddHealth(float HealthToAdd)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthToAdd, 0.0f, DefaultHealth);
}

void URHealthComponent::RespawnPlayer_Implementation()
{
}

void URHealthComponent::Die_Implementation(AActor* Instigator)
{
}


// Called when the game starts
void URHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}
