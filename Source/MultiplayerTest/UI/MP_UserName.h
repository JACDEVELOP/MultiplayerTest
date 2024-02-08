// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MP_BaseWidget.h"
#include "Components/TextBlock.h"
#include "MP_UserName.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UMP_UserName : public UMP_BaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(const FText NewUserName) const {UserName->SetText(NewUserName);}
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* UserName;
};
