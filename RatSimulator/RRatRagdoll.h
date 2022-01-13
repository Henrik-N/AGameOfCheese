// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "RRatRagdoll.generated.h"

class USkeletalMeshComponent;
class USceneComponent;

UCLASS()
class RATSIMULATOR_API ARRatRagdoll : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARRatRagdoll();

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
