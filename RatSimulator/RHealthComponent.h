// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RRatRagdoll.h"
#include "Components/ActorComponent.h"
#include "RHealthComponent.generated.h"

class AActor;
class ARRatRagdoll;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class RATSIMULATOR_API URHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	
	//Set this for maxhealth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float DefaultHealth;

	//In Seconds. How often health should regenerate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regeneration")
	float HealthRegenRate;

	//How much Health should be regenerated 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regeneration")
	float HealthToRegenerate;

	//How long(seconds) it should take before starting to regenerate health after damage 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regeneration")
	float DamageTimer;

	//The health the character uses, startvalue is DeafaultHealth
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CurrentHealth;

	//Time past since the player took damage
	UPROPERTY(BlueprintReadOnly)
	float TimeSinceLastDamage;

	//Player taking damage
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(float Damage, AActor* Instigator);

	//Add Health to the player
	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthToAdd);

	//Method for handling death of player
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die(AActor* Instigator);

	//Method for handling respawn of player
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RespawnPlayer();

	//Reference to ragdoll of player
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARRatRagdoll> RatRagdollClass;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Counter for regeneration rate of hp
	UPROPERTY()
	float HealthTimer = 0.f;

	//For debugging
	UPROPERTY(EditAnywhere)
	bool bShowDebug = false;
	
};


