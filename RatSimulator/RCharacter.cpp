#include "RCharacter.h"
#include "RCharacterMovementComponent.h"
#include "RVaultingComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "RPlayerSoundMaker.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


ARCharacter::ARCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	

	
	SoundsComponent = CreateDefaultSubobject<URPlayerSoundMaker>(TEXT("SoundMaker"));
	VaultingComponent = CreateDefaultSubobject<URVaultingComponent>(TEXT("Vaulting component"));
	HealthComponent = CreateDefaultSubobject<URHealthComponent>(TEXT("HealthComponent"));
	InteractComponent = CreateDefaultSubobject<URInteractComponent>(TEXT("InteractComponent"));

	
	// TICK SETTINGS
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultCameraZOffset = CameraComponent->GetRelativeLocation().Z;
	
}

#pragma region Input


void ARCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARCharacter::OnJumpKeyDown);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARCharacter::OnJumpKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARCharacter::OnSprintKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARCharacter::OnSprintKeyUp);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARCharacter::OnCrouchKeyDown);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ARCharacter::OnCrouchKeyUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARCharacter::Interact);

	PlayerInputComponent->BindAction("SmellVision", IE_Pressed, this, &ARCharacter::OnSmellKeyDown);
	PlayerInputComponent->BindAction("SmellVision", IE_Released, this, &ARCharacter::OnSmellKeyUp);
	

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ARCharacter::Interact()
{
	InteractComponent->InteractPressed();
}


void ARCharacter::MoveForward(float Value)
{
	Cast<URCharacterMovementComponent>(GetMovementComponent())->MoveForward(Value);
}

void ARCharacter::MoveRight(float Value)
{
	Cast<URCharacterMovementComponent>(GetMovementComponent())->MoveRight(Value);
}

void ARCharacter::OnSprintKeyDown()
{
	Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnSprintKeyDown();
	UE_LOG(LogTemp, Warning, TEXT("Sprint key down"))
}

void ARCharacter::OnSprintKeyUp()
{
	Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnSprintKeyUp();
	UE_LOG(LogTemp, Warning, TEXT("Sprint key up"))
}

void ARCharacter::OnCrouchKeyDown()
{
	Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnCrouchKeyDown();

	UE_LOG(LogTemp, Warning, TEXT("Crouch key down"))
}

void ARCharacter::OnCrouchKeyUp()
{
	Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnCrouchKeyUp();
	UE_LOG(LogTemp, Warning, TEXT("Crouch key up"))
}

void ARCharacter::OnJumpKeyDown()
{
	
	if (/*!VaultingComponent->TryVault() ||*/ !VaultingComponent->bIsVaulting)
	{
		Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnChargeJumpKeyDown();
	}
}

void ARCharacter::OnJumpKeyUp()
{
	if (!VaultingComponent->bIsVaulting)
	{
		Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnChargeJumpKeyUp();
	}
}

void ARCharacter::OnSmellKeyDown()
{
	if (!VaultingComponent->bIsVaulting)
	{
		Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnSmellKeyDown();
	}
}

void ARCharacter::OnSmellKeyUp()
{
	if (!VaultingComponent->bIsVaulting)
	{
		Cast<URCharacterMovementComponent>(GetCharacterMovement())->OnSmellKeyUp();
	}
}

#pragma endregion