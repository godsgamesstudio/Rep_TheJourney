// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Engine/World.h" // Necessário para GetWorld()
#include "TimerManager.h" // Necessário para FTimerHandle

#include "CharReactions.generated.h"


UCLASS(Blueprintable) 
class CHARACTERSTATESMANAGER_API UCharReactions : public UObject
{
	GENERATED_BODY()

public:

// FUNCTIONS
	// Ativar os Timers
	virtual UWorld* GetWorld() const override;

	// Funções normais para C++
	UFUNCTION()
	void Initialize (class APawn* Pawn_Ref, class UCharStatesManager* CSM_Ref, FGameplayTagContainer Groups, FGameplayTagContainer Tags, FGameplayTagContainer BlockedTags);
	UFUNCTION()
	bool VerifyReaction();
	UFUNCTION()
	void Enter(class UObject* ActorCauser, FGameplayTagContainer PayloadTags);
	UFUNCTION()
	void CallPersistentTick();
	UFUNCTION()
	void CancelReaction(TSubclassOf <UCharReactions> NewReaction);
	UFUNCTION()
	bool GetCanMovement();


protected:
// UPROPERTY Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Persistent Tick Function", 
		meta=(AllowPrivateAccess="true"), meta = (ToolTip = "Enables the 'OnPersistentTick' function"))
	bool EnableTickUpdate = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Persistent Tick Function", 
		meta=(AllowPrivateAccess="true"), meta = (ToolTop = "Determines the interval between ticks of the 'OnPersistentTick' event"))
	float PersistentTick_Interval = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement Setup", 
		meta=(AllowPrivateAccess="true"), meta = (ToolTop = "Allows character movement input while this occurs."))
	bool EnableCharMovement = false;

	UPROPERTY(BlueprintReadOnly, Category="Basic Setup", 
		meta=(AllowPrivateAccess="true"))
	class APawn* Pawn_Owner;

	UPROPERTY(BlueprintReadOnly, Category="Basic Setup", 
		meta=(AllowPrivateAccess="true"))
	class UCharStatesManager* CSM_Owner;

// Variables
	UPROPERTY()
	FTimerHandle PersistentTick;
	
	//Groups & Tags
	UPROPERTY(BlueprintReadOnly, Category="GroupsAndTags")
	FGameplayTagContainer MyGroups;
	UPROPERTY(BlueprintReadOnly, Category="GroupsAndTags")
	FGameplayTagContainer MyTags;
	UPROPERTY(BlueprintReadOnly, Category="GroupsAndTags")
	FGameplayTagContainer MyBlockedTags;
	
// FUNCTIONS TO USER USE
	UFUNCTION(BlueprintCallable, Category = "CSM")	
	void StopReaction();

// BP FUNCTIONS TO OVERRIDE
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "//"))
	bool ConditionToEnter();
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "//"))
	void OnEnter(class UObject* ActorCauser, FGameplayTagContainer PayloadTags);
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "//"))
	void OnPersistentTick(float IntervalTime);
	UFUNCTION(BlueprintNativeEvent, meta = (ToolTip = "//"))
	void OnCanceled(TSubclassOf <UCharReactions> NewReaction);
	
private:

	
};
