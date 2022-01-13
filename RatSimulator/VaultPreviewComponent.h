// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "VaultPreviewComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RATSIMULATOR_API UVaultPreviewComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	//-1 = UntilDisabled
	UFUNCTION(BlueprintCallable)
	void ActivatePreview(FVector Pos, float TimeUntilDisabled = -1.f);
	UFUNCTION(BlueprintCallable)
	void DisablePreview();
protected:
	FTimerHandle DisableTimer;
};
