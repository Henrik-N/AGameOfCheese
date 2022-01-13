#pragma once

#include "CoreMinimal.h"


#include "RHealthComponent.h"
#include "RInteractComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "RCharacter.generated.h"

class URPlayerSoundMaker;
class URDA_CheeseSettings;
class URInteractComponent;
class URHealthComponent;
class URVaultingComponent;
class URDA_CharacterMovementSettings;
class UCameraComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class RATSIMULATOR_API ARCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// APhysics_FPSCharacter();
	explicit ARCharacter(const class FObjectInitializer& ObjectInitializer);

	// Tick
	virtual void Tick(float DeltaSeconds) override;

	//// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URVaultingComponent* VaultingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPlayerSoundMaker* SoundsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URInteractComponent* InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	URDA_CheeseSettings* CheeseDataSettings;


public:
	UPROPERTY(BlueprintReadOnly)
	float DefaultCameraZOffset = 0.f;

	UPROPERTY(BlueprintReadWrite)
	int ConsumedMeth = 0;
	
protected:
	virtual void BeginPlay() override;

#pragma region Input
protected:
	// INPUT
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
private:

	void Interact();

	// Movement
	void MoveForward(float Val);
	void MoveRight(float Val);
	void OnSprintKeyDown();
	void OnSprintKeyUp();
	void OnCrouchKeyDown();
	void OnCrouchKeyUp();
	void OnJumpKeyDown();
	void OnJumpKeyUp();
	void OnSmellKeyDown();
	void OnSmellKeyUp();
	
#pragma endregion
};
