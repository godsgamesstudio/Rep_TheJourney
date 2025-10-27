// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"

#include "CSM_Structures.generated.h"

class UCharMasterState;
class UCharSpecificState;
class UCharActions;
class UCharReactions;

///////////// STRUCTURES ////////////////

// SETUP DE AÇÕES
USTRUCT(BlueprintType)
struct FStrc_Act_Setup
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager", 
	meta = (ToolTip = "Essential: Enter the Input Action that activates this action."))
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Essential: Enter all Specific States that this action can be used."))
	TArray <TSubclassOf <UCharSpecificState>> CanUseIn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: Here you can set which other actions this action can interrupt."))
	TArray <TSubclassOf <UCharActions>> CanInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple groups, but only 1 is active at a time.")) 
	FGameplayTagContainer Groups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple Tags, all tags need to be enabled for this to work.")) 
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: Blocks this Action if any of the configured tags are activated.")) 
	FGameplayTagContainer BlockedTags;
};

// SETUP DE ESTADOS ESPECÍFICOS
USTRUCT(BlueprintType)
struct FStrc_SS_Setup
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Char States Manager",
	meta = (ToolTip = "Essential: What other Specific States can be called from this one? (Only those that are part of this Master State)"))
	TSet <TSubclassOf <UCharSpecificState>> CanEnterIn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple groups, but only 1 is active at a time.")) 
	FGameplayTagContainer Groups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple Tags, all tags need to be enabled for this to work.")) 
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: Blocks this state if any of the configured tags are activated.")) 
	FGameplayTagContainer BlockedTags;
};

// SETUP DE ESTADOS MESTRES
USTRUCT(BlueprintType)
struct FStrc_MS_Setup
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Char States Manager",
	meta = (ToolTip = "Essential: What other Master States can be called from this one?"))
	TSet <TSubclassOf <UCharMasterState>> CanEnterIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Char States Manager",
	meta = (ToolTip = "Essential: Here you can configure the Specific States that are part of this Master State."))
	TMap <TSubclassOf<UCharSpecificState>, FStrc_SS_Setup> SpecificStates;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple groups, but only 1 is active at a time.")) 
	FGameplayTagContainer Groups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple Tags, all tags need to be enabled for this to work.")) 
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: Blocks this state if any of the configured tags are activated.")) 
	FGameplayTagContainer BlockedTags;
};

USTRUCT(BlueprintType)
struct FStrc_Ract_Setup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: Here you can set which other actions this action can interrupt."))
	TArray <TSubclassOf <UCharReactions>> CanInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple groups, but only 1 is active at a time.")) 
	FGameplayTagContainer Groups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: You can create or use multiple Tags, all tags need to be enabled for this to work.")) 
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Char States Manager",
	meta = (ToolTip = "Optional: Blocks this Action if any of the configured tags are activated.")) 
	FGameplayTagContainer BlockedTags;
};


// SAVE
USTRUCT(BlueprintType)
struct FCSM_SaveInfos
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CSM Save",
	meta = (ToolTip = "Gameplay Tag")) 
	FGameplayTag ActiveGroup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CSM Save",
	meta = (ToolTip = "Gameplay Tag Container")) 
	FGameplayTagContainer ActiveTags;
};

///////////// ENUMERATORS ////////////////

// ENUM PARA DIFERENCIAR ERROS DE AÇÃO
UENUM(BlueprintType)
enum class EActionErrorType : uint8
{
	AER_ConditionFail
	UMETA(DIsplayName = "Condition Not Met"),
	AER_OtherAction
	UMETA(DisplayName = "Has Other Action")
};