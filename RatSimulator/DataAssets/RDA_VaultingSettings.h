// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RDA_VaultingSettings.generated.h"

UCLASS()
class RATSIMULATOR_API URDA_VaultingSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Vaulting)
	TEnumAsByte<ECollisionChannel> TraceChannel;
		
	UPROPERTY(EditAnywhere, Category = Vaulting)//How far up we can vault
	float MaxVaultHeight = 100.f; 
	UPROPERTY(EditAnywhere, Category = Vaulting)//If the final position is lower than this, make it just a step instead //TODO: hook up to step distance
	float MinVaultHeight = 30.f; 
	UPROPERTY(EditAnywhere, Category = Vaulting)//How far forward the vault takes us, suggest to set to a multiplier of the capsule size
	float VaultRange = 100.f; 
	UPROPERTY(EditAnywhere, Category = Vaulting)//How fast the vault goes at minimum
	float MinVaultSpeed = 0.5f;
	UPROPERTY(EditAnywhere, Category = Vaulting)//How fast the vault goes at max
	float MaxVaultSpeed = 5.f;
	UPROPERTY(EditAnywhere, Category = Vaulting)//How close to the end destination do we have to be before the vault is finished
	float VaultTolerance = 10.f; 
	UPROPERTY(EditAnywhere, Category = Vaulting)//This value impacts how far down from the roof the position starts. This is to prevent hitting the roof on the forward sweep.
	float RoofTolerance = 1.f; 
	UPROPERTY(EditAnywhere, Category = Vaulting)//This value controls how centered the view needs to be on the vaulting point
	float LookAtTreshold = 0.8f;
	UPROPERTY(EditAnywhere, Category = Vaulting)//Automatically try to vault when jumping
	bool bAutoVaultInAir;
	UPROPERTY(EditAnywhere, meta=(UIMin = 0.0, UIMax = 1.0), Category = Vaulting)//Beneath this point, use min end velocity
	float LowerRangeDot = 0.2f;
	UPROPERTY(EditAnywhere, meta=(UIMin = 0.0, UIMax = 1.0), Category = Vaulting)//Above this point use max end velocity
	float UpperRangeDot = 0.8f;
	UPROPERTY(EditAnywhere, meta=(UIMin = 0.0, UIMax = 1.0), Category = Vaulting) //Use to set minimum speed after vault no matter how far the vault is
	float MinEndVelocityMultiplier = 0.3f;
	UPROPERTY(EditAnywhere, meta=(UIMin = 0.0, UIMax = 1.0), Category = Vaulting) //Use to set max speed after vault no matter how short the vault is
	float MaxEndVelocityMultiplier = 1.0;
	UPROPERTY(EditAnywhere) //Use values higher than 1 for larger curve when vaulting
	float BezierHeightMultiplier = 1.0;
};
