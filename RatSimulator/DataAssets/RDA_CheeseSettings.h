// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RDA_CheeseSettings.generated.h"

/**
 * 
 */
UCLASS()
class RATSIMULATOR_API URDA_CheeseSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cheese")
	float LaunchSpeed = 500.f;
	
};
