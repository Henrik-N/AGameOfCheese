// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RStateInterface.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "RSmellStateComp.generated.h"


class UCameraComponent;
class UTimelineComponent;
class URCharacterMovementComponent;
class ARCharacter;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class RATSIMULATOR_API URSmellStateComp : public UActorComponent, public IRStateInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UTimelineComponent* TimelineComponent;
private:
	FOnTimelineFloat CameraZOffsetTimelineFloat;
	UFUNCTION()
	void CameraZOffsetTimeline(float Alpha);

	
public:
	// Sets default values for this component's properties
	URSmellStateComp();

	UPROPERTY(BlueprintReadOnly)
	ARCharacter* RCharacter;

	UPROPERTY(BlueprintReadOnly)
	URCharacterMovementComponent* RCharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* RCharacterCamera;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TimelineTick(float Alpha);

	
	UPROPERTY()
	float CameraZOffsetIntent = 0.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// STATE INTERFACE
	virtual void OnEnableState_Implementation() override;
	virtual void OnDisableState_Implementation(const TScriptInterface<IRStateInterface>& UpcomingState) override;
};
