// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RGameState.generated.h"

/**
 * 
 */
UCLASS()
class RATSIMULATOR_API ARGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	int ObjectiveTotal = 0;
	UPROPERTY(BlueprintReadOnly)
	int ObjectivesPickedUp = 0;

	UFUNCTION(BlueprintCallable) //Call this with your cheese object to add to the total
	void RegisterObjectives(int Total){ObjectiveTotal = Total;}
	UFUNCTION(BlueprintCallable)
	void ObjectivePickedUp(){++ObjectivesPickedUp;}
	UFUNCTION(BlueprintCallable)
    float GetObjectivePercent() {return ObjectivesPickedUp/static_cast<float>(ObjectiveTotal);}
};
