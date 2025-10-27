// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharStatesBase.h"
#include "CSM_Structures.h"
#include "Engine/World.h" // Necessário para GetWorld()
#include "TimerManager.h" // Necessário para FTimerHandle

#include "CharActions.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERSTATESMANAGER_API UCharActions : public UCharStatesBase
{
	GENERATED_BODY()

public:
// VARIABLES

	// To System
	UPROPERTY()
	FTimerHandle AutoExitTick;
	// To User
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actions Events Setup", meta=(AllowPrivateAccess="true"))
	bool EndOnInputRelease = false;

// FUNCTIONS
	UFUNCTION()
	bool VerifyAction();
	UFUNCTION()
	void CancelAction(TSubclassOf <UCharStatesBase> NewAction);
	
	// Override Function
	UFUNCTION(BlueprintNativeEvent)
	bool ActiveCondition();
	UFUNCTION(BlueprintNativeEvent)
	void OnCanceled(TSubclassOf <UCharStatesBase> NewAction);
	UFUNCTION(BlueprintNativeEvent)
	bool AutoExitVerifier();
	UFUNCTION(BlueprintnativeEvent)
	void OnActivateFail(EActionErrorType ErrorType);

	// Functions in Override
	virtual void Enter(TSubclassOf <UCharStatesBase> LastState) override;
private: 
// FUNCTIONS
	// Essa função serve apenas como ponto entre o Timer e a função de usuário "AutoExitVerifier"
	UFUNCTION()
	void AutoExitWrapper();

protected:
//FUNCTIONS TO USE
	UFUNCTION(BlueprintCallable, Category = "CSM")
	void StopAction();

// VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actions Events Setup", meta=(AllowPrivateAccess="true"))
	bool CallOnExitWhenCanceled = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actions Events Setup", meta=(AllowPrivateAccess="true"))
	bool EnableAutoExitFunction = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Actions Events Setup", meta=(AllowPrivateAccess="true"))
	float AutoExit_Interval = 0.01f;
	
};
