// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoxInteract.h"
#include "RDamageBox.generated.h"


UCLASS()
class RATSIMULATOR_API ARDamageBox : public ABoxInteractBase
{
	GENERATED_BODY()

public:

	ARDamageBox();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


	//The amount of damage to deal to the player
	UPROPERTY(EditAnywhere)
	float DamageToDeal;

	//Time between damage inside the box
	UPROPERTY(EditAnywhere)
	float TimeBetweenDamage = 1.f;

	//Actors who are inside the box
	UPROPERTY()
	TArray<AActor*> OverlappedActors;

	//Called when an actor leaves the box
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Test stuff
	UFUNCTION()
	void EmptyOverlappedActors();
	
	private:

	//Counter for when damage was last dealt by box
	UPROPERTY()
	float DamageTimeElapsed = 0.f;
};



