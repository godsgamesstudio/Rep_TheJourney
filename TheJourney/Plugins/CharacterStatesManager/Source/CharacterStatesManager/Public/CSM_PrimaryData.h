// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CSM_Structures.h"
#include "CSM_PrimaryData.generated.h"

class UCharMasterState;
class UCharSpecificState;
class UCharActions;
class UCharReactions;

/**
 * 
 */
UCLASS(NotBlueprintable)
class CHARACTERSTATESMANAGER_API UCSM_PrimaryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
// Variables
    //Setup Character State Manager
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groups and RequiredTags") 
	FGameplayTag DefaultActiveGroup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groups and RequiredTags") 
	FGameplayTagContainer DefaultTagStates;

    UPROPERTY(EditAnywhere, Category = "Char States Manager")
    TMap <TSubclassOf<UCharMasterState>, FStrc_MS_Setup> MasterStates;

    UPROPERTY(EditAnywhere, Category = "Char States Manager")
    TMap <TSubclassOf <UCharActions>, FStrc_Act_Setup> Actions;

    UPROPERTY(EditAnywhere, Category = "Char States Manager")
    TMap <TSubclassOf <UCharReactions>, FStrc_Ract_Setup> Reactions;

// Functions
    // Get Classes
    UFUNCTION()
    TArray <TSubclassOf <UCharMasterState>> GetMasterStates();
    UFUNCTION()
    TSet <TSubclassOf <UCharMasterState>> Get_MS_CanEnter(TSubclassOf <UCharMasterState> MasterState);
    
    UFUNCTION()
    TArray <TSubclassOf <UCharSpecificState>> GetSpecicicStates(TSubclassOf <UCharMasterState> MasterState);
    UFUNCTION()
    TSet <TSubclassOf <UCharSpecificState>> Get_SS_CanEnter(TSubclassOf <UCharMasterState> MasterState, TSubclassOf <UCharSpecificState> SpecificState);

    UFUNCTION()
    TArray <TSubclassOf <UCharActions>> GetActions();

    UFUNCTION()
    TArray <TSubclassOf <UCharReactions>> GetReactions();

    // Get Infos
    UFUNCTION()
    FStrc_MS_Setup GetMasterStateInfos(TSubclassOf <UCharMasterState> MasterState) const;
    UFUNCTION()
    FStrc_SS_Setup GetSpecificStateInfos(TSubclassOf <UCharMasterState> MasterState, TSubclassOf <UCharSpecificState> SpecificState) const;
    UFUNCTION()
    FStrc_Act_Setup GetActionInfo(TSubclassOf <UCharActions> Action);
    UFUNCTION()
    FStrc_Ract_Setup GetReactionInfo(TSubclassOf <UCharReactions> Reaction);
    
    
    UFUNCTION()
    TSubclassOf <UCharActions> GetActionsBySS(TSubclassOf<UCharSpecificState> SpecificState, const UInputAction* ActionInput);
    


private:


};
