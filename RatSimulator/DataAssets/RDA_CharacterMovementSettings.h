
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RatSimulator/MovementStates/RClimbStateComp.h"
#include "RatSimulator/MovementStates/RCrouchStateComp.h"
#include "RatSimulator/MovementStates/RRunStateComp.h"
#include "RatSimulator/MovementStates/RSlideStateComp.h"
#include "RatSimulator/MovementStates/RSmellStateComp.h"
#include "RatSimulator/MovementStates/RVaultStateComp.h"
#include "RatSimulator/MovementStates/RWalkStateComp.h"

#include "RDA_CharacterMovementSettings.generated.h"


/**
* 
*/


UCLASS()
class RATSIMULATOR_API URDA_CharacterMovementSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Smelling")
	UCurveFloat* SmellCameraFloatCurveAlpha;

	UPROPERTY(EditDefaultsOnly, Category = "Smelling")
	float CameraSmellZOffset = 150.f;

	/// CLIMBING
	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float ForwardClimbSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float RightClimbSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float LeapForce = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Climbing", meta=(ClampMin = "-89.9", ClampMax = "89.9", UIMin = "-89.9", UIMax = "89.9"))
	float MinCameraPitchClimbing = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Directional Movement Fractions", meta=(ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ForwardMovementFraction = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Directional Movement Fractions", meta=(ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BackwardsMovementFraction = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Directional Movement Fractions", meta=(ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SidewaysMovementFraction = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Walking")
	float WalkSpeed = 700.f;

	/// SPRINTING
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprinting")
	// float SprintingSpeed = 1500.f;

	/// CROUCHING
	UPROPERTY(EditDefaultsOnly, Category = "Crouching / Jumping")
	UCurveFloat* CrouchCameraFloatCurveAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouching / Jumping")
	float CameraCrouchZOffset = -60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Crouching / Jumping")
	UCurveFloat* JumpChargeCurve;

	
	UPROPERTY(EditDefaultsOnly, Category = "Crouching / Jumping")
	UCurveFloat* JumpChargeUpgradeInfluenceOverJumpChargeCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Crouching / Jumping")
	float JumpChargeIncreasePerJumpChargeUpgrade = 0.f;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Crouching / Jumping")
	float CoyoteTimeGracePeriod = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crouching / Jumping")
	float CrouchMovementSpeed = 450.f;


	/// SLIDING
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sliding")
	float SlideMultiplier = 70000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sliding")
	float MaxSlideSpeed = 1500.f;


#pragma region States
	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URWalkStateComp> WalkState1Component;

	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URRunStateComp> RunState1Component;

	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URCrouchStateComp> CrouchState1Component;

	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URSlideStateComp> SlideState1Component;

	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URVaultStateComp> VaultState1Component;

	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URClimbStateComp> ClimbState1Component;

	UPROPERTY(EditDefaultsOnly, Category = "StateMachine")
	TSubclassOf<URSmellStateComp> SmellState1Component;
	
#pragma endregion

	
};
