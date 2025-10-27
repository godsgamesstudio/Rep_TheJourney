// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharStatesBase.h"
#include "Engine/World.h" // Necessário para GetWorld()
#include "TimerManager.h" // Necessário para FTimerHandle
#include "CharSpecificState.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERSTATESMANAGER_API UCharSpecificState : public UCharStatesBase
{
	GENERATED_BODY()

public:
//FUNCTIONS
	// To C++
  	UFUNCTION()
	bool VerifySpecificState();

	//To Override
	UFUNCTION(BlueprintNativeEvent)
	bool StartCondition();

	//To User
	UFUNCTION(BlueprintCallable, meta = (tooltip = "//"), Category="Movement Setup")
	bool GetCanCharacterMove();
	UFUNCTION()
	bool GetCanSwitchProfile();

protected:
//VARIABLES
	//Setup Specific State
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup", meta = (tooltip = "Control whether the character can move while in this state."))
	bool CanCharacterMove = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup", meta = (tooltip = "Allows you to change your PROFILE while in this state."))
	bool CanSwitchProfile = true;
};
