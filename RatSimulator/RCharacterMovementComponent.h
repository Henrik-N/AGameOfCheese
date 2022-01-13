#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RCharacterMovementComponent.generated.h"

class URVaultStateComp;
class URClimbStateComp;
class URSmellStateComp;
class URCrouchStateComp;
class URWalkStateComp;
class IRStateInterface;
class UCameraComponent;
class ARCharacter;
class UTimelineComponent;
class URDA_CharacterMovementSettings;
class URMovementState;



/**
**
**/

UCLASS()
class RATSIMULATOR_API URCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	// INIT
	explicit URCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
private:
	void InitBpClasses();
	template<class T>
	T* InitScriptInterface(TScriptInterface<IRStateInterface>& Interface, TSubclassOf<T> Object);
	virtual void BeginPlay() override;

public:
	/// SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	URDA_CharacterMovementSettings* DataSettings;

// SUBOBJECTS
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// todo? Wallrun
	// UFUNCTION(BlueprintCallable)
	// void CheckForWallrunTick(float DeltaTime);



	// todo override "PhysFalling" function, if top of capsule hits a border,
	

	
	

public:
#pragma region Input
	/// INPUT
	UFUNCTION(BlueprintCallable)
	void SetMovementInputEnabled(bool bInInputEnabled);

	// cached input axis
	UPROPERTY(BlueprintReadOnly)
	FVector2D InputAxis = FVector2D::ZeroVector;

	UFUNCTION(BlueprintCallable)
	FVector2D GetCachedInputAxis() const;
	
	UFUNCTION()
	void MoveForward(float Value);
	
	UFUNCTION()
	void MoveRight(float Value);
	void OnSprintKeyDown();
	
	void OnSprintKeyUp();
	void OnCrouchKeyDown();
	void OnCrouchKeyUp();
	void OnSmellKeyDown();
	void OnSmellKeyUp();

	// JUMP
	void OnChargeJumpKeyDown();
	void OnChargeJumpKeyUp();

	UPROPERTY()
	bool bSprintKeyDown = false;
	UPROPERTY()
	bool bCrouchKeyDown = false;
	UPROPERTY()
	bool bJumpChargeKeyDown = false;
	UPROPERTY()
	bool bSmellKeyDown = false;
	
private:
	UPROPERTY()
	bool bInputEnabled = true;
	
#pragma endregion 

public:
#pragma region CLIMBING
	/// CLIMBING
	///
	TOptional<FHitResult> RaycastFromChar(const FVector& Direction, float RayDistance) const;

	UFUNCTION()
	bool CanClimb();

	UFUNCTION(BlueprintCallable)
	FVector CalculateFloorSlopeInfluence() const;

	UFUNCTION(BlueprintCallable)
	void SlideClampVelocity();

	UFUNCTION(BlueprintCallable)
	void ChangeStateIfTooSlow();
	
	UFUNCTION()
	bool CanStandUp() const;
#pragma endregion
	
private:
	UFUNCTION()
	void OnPawnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
#pragma region VAULT
	UFUNCTION()
	void OnStartVault();
	UFUNCTION()
	void OnEndVault();
	
	UFUNCTION(BlueprintCallable)
	void ResolveMovementState();
#pragma endregion

public:
#pragma region CachedValues

	UPROPERTY()
	ARCharacter* RCharacter;

	UPROPERTY()
	UCameraComponent* RCharacterCameraComp;
	
	// UPROPERTY(BlueprintReadOnly)
	// float StandingCameraZOffset;
	
	UPROPERTY(BlueprintReadWrite)
	float DefaultGravityScale;

	UPROPERTY(BlueprintReadOnly)
	float StandingCapsuleHalfHeight;
	
	UPROPERTY(BlueprintReadOnly)
	float StandingMeshHeight;

	
	
#pragma endregion
	

#pragma region States

public:
	UFUNCTION(BlueprintCallable)
	void ChangeState(const TScriptInterface<IRStateInterface>& InNewState);
	
protected:
	// CURRENT STATE
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> CurrentStateComponent;
public:
	TScriptInterface<IRStateInterface> GetCurrentState() const;
	
	// WALK STATE
	UPROPERTY()
	UActorComponent* WalkStateComp;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> WalkState1;

	// RUN STATE
	// UPROPERTY()
	// UActorComponent* RunStateComp;
	// UPROPERTY(BlueprintReadOnly)
	// TScriptInterface<IRStateInterface> RunState1;

	// CROUCH STATE
	UPROPERTY()
	URCrouchStateComp* CrouchStateComp;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> CrouchState1;

	// SLIDE STATE
	UPROPERTY()
	UActorComponent* SlideStateComp;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> SlideState1;

	// VAULT STATE
	UPROPERTY()
	URVaultStateComp* VaultStateComp;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> VaultState1;

	// CLIMB STATE
	UPROPERTY()
	URClimbStateComp* ClimbStateComp;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> ClimbState1;

	// SMELL-O-VISION
	UPROPERTY()
	URSmellStateComp* SmellStateComp;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IRStateInterface> SmellState1;
	
	
#pragma endregion

#pragma region Utility
public:
	UFUNCTION(BlueprintCallable)
	float GetMovementPercent() const;

	UFUNCTION(BlueprintCallable)
	float RCalculateDirectionClimbing(const FVector& RVelocity, const FRotator& BaseRotation) const;
	
#pragma endregion 
	
};
