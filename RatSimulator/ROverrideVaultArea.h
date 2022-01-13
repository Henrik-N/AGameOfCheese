// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROverrideVaultArea.generated.h"

class UBoxComponent;
UCLASS()
class RATSIMULATOR_API AROverrideVaultArea : public AActor
{
	GENERATED_BODY()

//Built-in functions
public:
	AROverrideVaultArea();
protected:
	virtual void BeginPlay() override;

//Protected functions
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
    void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

//Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* VaultDestination;

};
