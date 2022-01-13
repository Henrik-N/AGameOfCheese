#pragma once
#include "Components/BoxComponent.h"


#include "BoxInteract.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

//Baseclass for box interactions
UCLASS()
class ABoxInteractBase : public AActor
{
	GENERATED_BODY()
public:

	ABoxInteractBase();

	virtual void BeginPlay() override;

	//Rootcomponent
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	//Mesh component 
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	//Box component
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UFUNCTION()
    virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	
};
