// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Chaos/ChaosEngineInterface.h"
#include "RPlayerSoundMaker.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RATSIMULATOR_API URPlayerSoundMaker : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EPhysicalSurface>,USoundBase*> Sounds;
	UPROPERTY(EditDefaultsOnly) //For LineTrace to get surface type
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

protected:
	void SpawnSound(EPhysicalSurface Surface);
public:
	UFUNCTION(BlueprintCallable)
	void PlayFootstep();
};
