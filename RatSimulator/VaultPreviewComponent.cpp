// Fill out your copyright notice in the Description page of Project Settings.


#include "VaultPreviewComponent.h"

void UVaultPreviewComponent::ActivatePreview(FVector Pos, float TimeUntilDisabled)
{
	GetWorld()->GetTimerManager().ClearTimer(DisableTimer);
	if(TimeUntilDisabled > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DisableTimer, this, &UVaultPreviewComponent::DisablePreview, TimeUntilDisabled, false);
	}
	SetVisibility(true);
	SetWorldLocation(Pos);
}

void UVaultPreviewComponent::DisablePreview()
{
	GetWorld()->GetTimerManager().ClearTimer(DisableTimer);
	SetVisibility(false);
}
