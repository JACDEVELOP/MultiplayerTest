// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTest/Items/MP_InteractableItem.h"
#include "MP_DoorBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMP_DoorInteraction);

UCLASS()
class MULTIPLAYERTEST_API AMP_DoorBase : public AMP_InteractableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_DoorBase();

	UFUNCTION(Server, Reliable)
	void Server_HandleDoorInteraction();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HandleDoorInteraction();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySound();

	UFUNCTION(Server, Reliable)
	void Server_ToggleDoor();
	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interactuable")
	uint8 bIsLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interactuable")
	USoundBase* DoorSound;

	UPROPERTY(BlueprintAssignable)
	FMP_DoorInteraction DoorInteractionDelegate;

	virtual void Tick(float DeltaSeconds) override;
};

