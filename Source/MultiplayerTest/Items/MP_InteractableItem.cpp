// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_InteractableItem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMP_InteractableItem::AMP_InteractableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(StaticMeshComponent);
	WidgetComponent->SetVisibility(false);
	WidgetComponent->SetIsReplicated(true);

	bReplicates = true;
	NetDormancy = DORM_DormantAll;
	bCanInteract = true;
}


void AMP_InteractableItem::Interact_Implementation()
{
	IMP_InteractInterface::Interact_Implementation();
}

void AMP_InteractableItem::EnableInteractWidget_Implementation(const bool bIsEnableWidget)
{
	IMP_InteractInterface::EnableInteractWidget_Implementation(bIsEnableWidget);
	WidgetComponent->SetVisibility(bIsEnableWidget);
}

// Called every frame
void AMP_InteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector CameraManagerLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	const FVector WidgetLocation = WidgetComponent->GetComponentLocation();
	const FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(WidgetLocation,CameraManagerLocation);
	WidgetComponent->SetWorldRotation(PlayerRot);
}

