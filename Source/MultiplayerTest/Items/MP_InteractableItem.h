// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "MultiplayerTest/Interfaces/MP_InteractInterface.h"
#include "MP_InteractableItem.generated.h"

UCLASS()
class MULTIPLAYERTEST_API AMP_InteractableItem : public AActor, public  IMP_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_InteractableItem();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interactuable")
	uint8 bCanOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interactuable")
	uint8 bCanInteract;

	virtual void Interact_Implementation() override;

	virtual void EnableInteractWidget_Implementation(const bool bIsEnableWidget) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
