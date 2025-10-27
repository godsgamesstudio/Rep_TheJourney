// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "CharStatesManager.generated.h"

class APawn;
class UCSM_PrimaryData;
class UCharMasterState;
class UCharSpecificState;
class UCharActions;
class UCharReactions;

// DISPATCHERS

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMasterStateChangedSignature, TSubclassOf <UCharMasterState>, NewMasterState, TSubclassOf <UCharMasterState>, OldMasterState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpecificStateChangedSignature, TSubclassOf <UCharSpecificState>, NewSpecificState, TSubclassOf <UCharSpecificState>, OldSpecificState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionEntrySignature, TSubclassOf <UCharActions>, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionExitSignature, TSubclassOf <UCharActions>, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProfileChanged, FGameplayTag, EnterProfile, FGameplayTag, ExitProfile);

UCLASS( NotBlueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisallowMultipleComponent))
class CHARACTERSTATESMANAGER_API UCharStatesManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharStatesManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// VARIABLES
	UPROPERTY() 
	FGameplayTag ActiveGroup;
	UPROPERTY() 
	FGameplayTagContainer TagStates;


	// FUNCTIONS To User
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "It is used to initiate an action. It requires an Input Action input."))
	bool PlayAction(UInputAction* ActionInput);
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "//"))
	bool PlayReaction(TSubclassOf <UCharReactions> Reaction, class UObject* CauserActor, FGameplayTagContainer PayloadTags);
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "Stops the current action if it exists and if it allows the 'EndOnInputRelease' interaction."))
	bool ReleaseButton();
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "It is used to check if the character can move."))
	bool CanCharacterMove();

	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "Changes the currently active group, only 1 group remains active at a time."))
	bool SetActiveGroup(FGameplayTag GroupTag);
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "Changes the status of a specific tag. Multiple tags can be active at the same time."))
	bool SetActiveTag(FGameplayTag Tag, bool Active);
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "Defines the complete tag container."))
	void SetTagStates(FGameplayTagContainer TagContainer);

	UFUNCTION(BlueprintPure, Category="CSM System", meta = (ToolTip = "Get all the important information from the CSM. Useful for saving character settings."))
	FCSM_SaveInfos GetSaveInfos() const;
	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "Sets all important CSM information. Useful for restoring character settings."))
	void SetSaveInfos(FCSM_SaveInfos SaveInfos);

	UFUNCTION(BlueprintCallable, Category="CSM System", meta = (ToolTip = "//."))
	bool SwitchProfile(FGameplayTag Profile);
	
	
	// Pure Functions
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the class of the currently active Master State."))
	TSubclassOf <UCharMasterState> GetCurrentMasterState() const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the class of the currently active Specific State."))
	TSubclassOf <UCharSpecificState> GetCurrentSpecificState() const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the class of the currently active Action."))
	TSubclassOf <UCharActions> GetCurrentAction() const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the class of the currently active Reaction."))
	TSubclassOf <UCharReactions> GetCurrentReaction() const;

	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the Master State instance reference using its class. Returns 'null' if it doesn't exist."))
	UCharMasterState* GetMasterStateInstance(TSubclassOf <UCharMasterState> MasterState) const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the instance reference of Specific State using its class. Returns 'null' if it does not exist."))
	UCharSpecificState* GetSpecificStateInstance(TSubclassOf <UCharSpecificState> SpecificState) const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the Action instance reference using its class. Returns 'null' if it doesn't exist."))
	UCharActions* GetActionInstance(TSubclassOf <UCharActions> Action) const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Gets the Reaction instance reference using its class. Returns 'null' if it doesn't exist."))
	UCharReactions* GetReactionInstance(TSubclassOf <UCharReactions> Reaction) const;

	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Returns the Gameplay Tag of the currently active group."))
	FGameplayTag GetCurrentGroup() const;
	UFUNCTION(BlueprintPure, Category="CSM Functions", meta = (ToolTip = "Returns the Gameplay Tag Container of the currently active Tags."))
	FGameplayTagContainer GetCurrentTags() const;

// FUNCTIONS To System
	UFUNCTION()
	void StopAction();

	UFUNCTION()
	void StopReaction();

	UFUNCTION()
	void ExitAllStates();

// DISPATCHERS
	UPROPERTY(BlueprintAssignable, Category = "CSM|Events", meta = (ToolTip = "It is triggered when the Master State is changed."))
	FOnMasterStateChangedSignature OnMasterStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "CSM|Events", meta = (ToolTip = "It is triggered when the Specific State is changed."))
	FOnSpecificStateChangedSignature OnSpecificStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "CSM|Events", meta = (ToolTip = "It is triggered when an Action is executed, it signals with the name of the action."))
	FOnActionEntrySignature OnActionEntry;

	UPROPERTY(BlueprintAssignable, Category = "CSM|Events", meta = (ToolTip = "It is triggered when an Action is finished, it signals with the name of the action."))
	FOnActionExitSignature OnActionExit;

	UPROPERTY(BlueprintAssignable, Category = "CSM|Events", meta = (ToolTip = "It is triggered when an Action is finished, it signals with the name of the action."))
	FOnLoadingCompleted OnLoadingCompleted;

	UPROPERTY(BlueprintAssignable, Category = "CSM|Events", meta = (ToolTip = "It is called whenever the profile is changed."))
	FOnProfileChanged OnProfileChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



private:    
// VARIABLES
	// References
	UPROPERTY()
	TObjectPtr <APawn> Pawn_Owner;
	
	// States and Actions Objects Referenfe
	UPROPERTY()
    TMap <TSubclassOf <UCharMasterState>, TObjectPtr <UCharMasterState>> MS_Objs;
	UPROPERTY()
	TMap <TSubclassOf <UCharSpecificState>, TObjectPtr <UCharSpecificState>> SS_Objs;
	UPROPERTY()
	TMap <TSubclassOf <UCharActions>, TObjectPtr <UCharActions>> Act_Objs;
	UPROPERTY()
	TMap <TSubclassOf <UCharReactions>, TObjectPtr <UCharReactions>> Ract_Objs;
	
	// Current States and Location
	UPROPERTY()
	TSubclassOf <UCharMasterState> Current_MS;
	UPROPERTY()
	TSubclassOf <UCharSpecificState> Current_SS;
	UPROPERTY()
	TSubclassOf <UCharActions> Current_Act;
	UPROPERTY()
	TSubclassOf <UCharReactions> Current_Ract;

	UPROPERTY()
	TArray <TSubclassOf <UCharMasterState>> MS_Classes;
	UPROPERTY()
	TArray <TSubclassOf <UCharSpecificState>> SS_Classes;
	UPROPERTY()
	TArray <TSubclassOf <UCharActions>> Act_Classes;
	UPROPERTY()
	TArray <TSubclassOf <UCharReactions>> Ract_Classes;

	// Setup
	UPROPERTY(EditAnywhere, Category="Setup", meta = (ToolTip = "//"))
    TMap <FGameplayTag, TObjectPtr <UCSM_PrimaryData>> Profiles;
	UPROPERTY(EditAnywhere, Category="Setup", meta = (ToolTip = "//"))
    bool CanSwitchProfileInActionOrReaction = false;

	// Mechanism
    UPROPERTY()
	bool ComponentDisabled = false;
	UPROPERTY()
	FGameplayTag CSM_ProfileName;
	UPROPERTY()
	TObjectPtr <UCSM_PrimaryData> CSM_DataAsset;
	
	
	UPROPERTY()
	FString ERROR_Text = TEXT("CSM | Start!");

// FUNCTIONS
    UFUNCTION()
	void BeginPlayError(FString Text);

	
};
