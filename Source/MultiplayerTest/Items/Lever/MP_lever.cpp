// Fill out your copyright notice in the Description page of Project Settings.
#include "MP_lever.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMP_lever::AMP_lever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AMP_lever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMP_lever, AssignedDoor);
}

void AMP_lever::Interact_Implementation()
{
	Super::Interact_Implementation();
	if(!bCanInteract)
	{
		return;
	}
	
	if(!AssignedDoor.IsValid())
	{
		return;
	}
	
	AssignedDoor->Interact_Implementation();
}

void AMP_lever::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
