// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Engine/World.h" // Necessário para GetWorld()
#include "TimerManager.h" // Necessário para FTimerHandle
#include "CharStatesBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class CHARACTERSTATESMANAGER_API UCharStatesBase : public UObject
{
	GENERATED_BODY()

public:

// FUNCTIONS
	UFUNCTION()
	void Initialize (class APawn* Pawn_Ref, class UCharStatesManager* CSM_Ref, FGameplayTagContainer Groups, FGameplayTagContainer Tags, FGameplayTagContainer BlockedTags);

	UFUNCTION()
	virtual void Enter(TSubclassOf <UCharStatesBase> LastState);

	UFUNCTION()
	void CallPersistentTick();

	UFUNCTION()
	virtual void Exit(TSubclassOf <UCharStatesBase> NewState);
	
	// UWorld - Habilita o Delay e Timeline
	virtual UWorld* GetWorld() const override;

private:
// VARIABLES

// FUNCTIONS C++

// FUNCTIONS Blueprint


protected:
// UPROPERTY Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Persistent Tick Function", 
		meta=(AllowPrivateAccess="true"), meta = (ToolTip = "Enables the 'OnPersistentTick' function"))
	bool EnableTickUpdate = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Persistent Tick Function", 
		meta=(AllowPrivateAccess="true"), meta = (ToolTop = "Determines the interval between ticks of the 'OnPersistentTick' event"))
	float PersistentTick_Interval = 1.f;

	UPROPERTY(BlueprintReadOnly, Category="Basic Setup", meta=(AllowPrivateAccess="true"))
	class APawn* Pawn_Owner;

	UPROPERTY(BlueprintReadOnly, Category="Basic Setup", meta=(AllowPrivateAccess="true"))
	class UCharStatesManager* CSM_Owner;

// Variables
	UPROPERTY()
	FTimerHandle PersistentTick;
	
	// Groups & Tags
	UPROPERTY(BlueprintReadOnly, Category="GroupsAndTags")
	FGameplayTagContainer MyGroups;
	UPROPERTY(BlueprintReadOnly, Category="GroupsAndTags")
	FGameplayTagContainer MyTags;
	UPROPERTY(BlueprintReadOnly, Category="GroupsAndTags")
	FGameplayTagContainer MyBlockedTags;


// Functions To User
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the class of the currently active Master State."))
	TSubclassOf <UCharMasterState> GetCurrentMasterState() const;
	UFUNCTION(BlueprintPure, Category="CSm Functions", meta = (ToolTip = "Gets the class of the currently active Specific State."))
	TSubclassOf <UCharSpecificState> GetCurrentSpecificState() const;

// Override Functions
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "It is called when the state is called to enter."))
	void OnEnter(TSubclassOf <UCharStatesBase> LastState);

	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "It is called when the state is called to exit."))
	void OnExit(TSubclassOf <UCharStatesBase> NewState);

	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "It is called continuously while the state is active."))
	void OnPersistentTick(float IntervalTime);

// Manager Binds
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Notifies when the Master State changes."))
	void OnManagerChangedMasterState(TSubclassOf <UCharMasterState> NewMasterState, TSubclassOf <UCharMasterState> OldMasterState);
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Notifies when Specific State is changed."))
	void OnManagerChangedSpecificState(TSubclassOf <UCharSpecificState> NewSpecificState, TSubclassOf <UCharSpecificState> OldSpecificState);
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Notifies when an Action is performed."))
	void OnManagerActionEntry(TSubclassOf <UCharActions> Action);
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Notifies when an Action is completed."))
	void OnManagerActionExit(TSubclassOf <UCharActions> Action);
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "Notifies you when the CSM completes its initial setup."))
	void OnBeginPlay();

};
