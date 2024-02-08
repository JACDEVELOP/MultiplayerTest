// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_DoorBase.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMP_DoorBase::AMP_DoorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanOverlap = false;
	bCanInteract = true;
	bIsLocked = false;
}

void AMP_DoorBase::Multicast_HandleDoorInteraction_Implementation()
{
	DoorInteractionDelegate.Broadcast();
}

void AMP_DoorBase::Server_HandleDoorInteraction_Implementation()
{
	Multicast_PlaySound();
	Multicast_HandleDoorInteraction();
}

void AMP_DoorBase::Multicast_PlaySound_Implementation()
{
	if(!IsValid(DoorSound))
	{
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
}

void AMP_DoorBase::Server_ToggleDoor_Implementation()
{
	if(bCanOverlap && !bIsLocked)
	{
		Server_HandleDoorInteraction();
	}
}

void AMP_DoorBase::Interact_Implementation()
{
	Super::Interact_Implementation();
	if(bCanInteract && !bIsLocked)
	{
		Server_HandleDoorInteraction();
	}
}

void AMP_DoorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
