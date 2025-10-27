// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharStatesBase.h"
#include "CharMasterState.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERSTATESMANAGER_API UCharMasterState : public UCharStatesBase
{
	GENERATED_BODY()

public:

	// C++ FUNCTIONS
	UFUNCTION()
	bool VerifyMasterState();

	// FUNCTIONS FOR OVERRIDE
	UFUNCTION(BlueprintNativeEvent)
	bool StartCondition();


private:

	
};
