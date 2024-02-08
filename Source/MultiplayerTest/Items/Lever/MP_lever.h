// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTest/Items/MP_InteractableItem.h"
#include "MP_lever.generated.h"

UCLASS()
class MULTIPLAYERTEST_API AMP_lever : public AMP_InteractableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_lever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
