#include "BoxInteract.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ABoxInteractBase::ABoxInteractBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(Mesh);
	
}

void ABoxInteractBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxInteractBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}




