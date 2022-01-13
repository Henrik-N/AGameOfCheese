// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Category = Interacting)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RATSIMULATOR_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
#pragma region stuff
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interact)
	void OnFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interact)
	void OnFocusLost();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Interact)
	void OnInteract(AActor* Caller);
#pragma endregion 
	
};
