// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "RInteractComponent.generated.h"

class URInteractableComponent;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class RATSIMULATOR_API URInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//The range you can interact with stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Interacting)
	float InteractRange = 1000.f;

	//Debug
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDraw = false;

	UPROPERTY()
	UCameraComponent* Camera;

	FCollisionQueryParams FColParam;

	//Default trace from the camera 
	UFUNCTION()
	void TraceForward(FVector Origin, FVector Direction);

	//When interact bind is pressed
	UFUNCTION()
	void InteractPressed();

	//Used for when you dont want to interact with something
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldInteract = true;

private:
	//The actor that has been looked at most recently
	UPROPERTY()
	AActor* FocusedActor;
};
