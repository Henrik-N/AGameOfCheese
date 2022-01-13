
#include "ROverrideVaultArea.h"
#include "RVaultingComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AROverrideVaultArea::AROverrideVaultArea()
{
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	RootComponent = HitBox;

	VaultDestination = CreateDefaultSubobject<USceneComponent>(TEXT("Vaulting Destination"));
	VaultDestination->SetupAttachment(RootComponent);
}

void AROverrideVaultArea::BeginPlay()
{
	Super::BeginPlay();
	
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AROverrideVaultArea::HandleBeginOverlap);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AROverrideVaultArea::HandleEndOverlap);
}

void AROverrideVaultArea::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor)
		return;
	URVaultingComponent* VaultComp = OtherActor->FindComponentByClass<URVaultingComponent>();
	if(VaultComp)
	{
		//VaultComp->StartOverride(VaultDestination);
	}
}

void AROverrideVaultArea::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor)
		return;
	URVaultingComponent* VaultComp = OtherActor->FindComponentByClass<URVaultingComponent>();
	if(VaultComp)
	{
		//VaultComp->EndOverride(VaultDestination);
	}
}

