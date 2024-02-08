// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTest/Items/MP_InteractableItem.h"
#include "MultiplayerTest/Items/Door/MP_DoorBase.h"
#include "MP_lever.generated.h"

UCLASS()
class MULTIPLAYERTEST_API AMP_lever : public AMP_InteractableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_lever();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Interact_Implementation() override;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category= "Interactuable")
	TSoftObjectPtr<AMP_DoorBase> AssignedDoor;

	virtual void Tick(float DeltaSeconds) override;
};
