// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MP_BaseWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MP_MainMenu.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;
class UMP_MultiplayerSessionsSubSystem;
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEST_API UMP_MainMenu : public UMP_BaseWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPersonCPP/Maps/MAP_Lobby")));

protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result) const;
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();
	void MenuTearDown();
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	/*
	 * The subsystem designed to handle all online session functionality
	 */
	UPROPERTY()
	UMP_MultiplayerSessionsSubSystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections = 4;
	FString MatchType = TEXT("FreeForAll");
	FString PathToLobby = TEXT("");
};
