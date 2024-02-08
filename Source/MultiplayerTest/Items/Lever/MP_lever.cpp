// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_lever.h"


// Sets default values
AMP_lever::AMP_lever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMP_lever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMP_lever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

