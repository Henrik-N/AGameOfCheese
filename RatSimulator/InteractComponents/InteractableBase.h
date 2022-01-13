// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "InteractableBase.generated.h"

UCLASS()
class RATSIMULATOR_API AInteractableBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interact)
	void OnFocus();
	virtual void OnFocus_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interact)
	void OnFocusLost();
	virtual void OnFocusLost_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interact)
	void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller) override;

	
};
