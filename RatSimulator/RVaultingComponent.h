#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RatSimulator/DataAssets/RDA_VaultingSettings.h"
#include "RVaultingComponent.generated.h"

class URCharacterMovementComponent;
//class UVaultPreviewComponent;
class UCapsuleComponent;
class UCameraComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVaultDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FSweepDelegate, FHitResult, Hit, FVector, StartPos, FVector, EndPos, FQuat, Rot, bool, bValid);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RATSIMULATOR_API URVaultingComponent : public UActorComponent
{
	GENERATED_BODY()

//Built-in functions
public:	
	URVaultingComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsVaulting = false; //Used to determine if "vault animation/state" is in progress
	UPROPERTY(EditAnywhere)
	URDA_VaultingSettings* DataSettings;

//Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) //Use this and hook up the debug delegates to draw capsules for visualization every tick
	bool bFireDebugSweepEvents = true;
	
	UPROPERTY()
	UCapsuleComponent* CapsuleComponent = nullptr;
	//UPROPERTY()
	//UVaultPreviewComponent* PreviewComp;
	UPROPERTY()
	const UCameraComponent* CameraComponent;
	
	//UPROPERTY(BlueprintReadOnly)
	//bool bInsideOverrideArea = false;
	//UPROPERTY(BlueprintReadOnly)
	//const USceneComponent* OverrideDestination;

	UPROPERTY()
	URCharacterMovementComponent* MovementComponent;

	FVector MidPoint;
	FVector EndDestination = FVector::ZeroVector;


public:
//Delegates/Events
	UPROPERTY(BlueprintAssignable)
	FVaultDelegate OnStartVault;
	UPROPERTY(BlueprintAssignable)
	FVaultDelegate OnEndVault;
	UPROPERTY(BlueprintAssignable)
	FSweepDelegate OnVaultPossible;
	UPROPERTY(BlueprintAssignable)
	FSweepDelegate OnDebugSweep;

//Public methods

	UFUNCTION(BlueprintCallable) //Attempt to start a vault. This is the only proper way to begin vaulting
	bool TryVault(bool bHasToSeeDestination = true); 
	UFUNCTION(BlueprintCallable) //Verifies that the vault is possible. Three sweeps
	bool VaultPossible(bool bHasToSeeDestination = true);
	void EndVault(); 

	// UFUNCTION(BlueprintCallable) //Used when entering an override area
	// void StartOverride(const USceneComponent* SceneComp);
	// UFUNCTION(BlueprintCallable) //Used when exiting an override area
    // void EndOverride(const USceneComponent* OldOverrideDestination);

	FVector GetMidPoint() const {return MidPoint;}
	FVector GetEndDestination() const {return EndDestination;} 

//Internal methods
protected:
	void StartVault();
	bool CheckIfInView(FVector Destination) const;

	void CheckForAutoVault();
};
