// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractableBase.h"
#include "InteractableBall.generated.h"

UCLASS()
class RATSIMULATOR_API AInteractableBall : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBall();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnFocus_Implementation() override;

	virtual void OnFocusLost_Implementation() override;

	virtual void OnInteract_Implementation(AActor* Caller) override;
	
};
