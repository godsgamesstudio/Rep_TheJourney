// Fill out your copyright notice in the Description page of Project Settings.


#include "CharActions.h"
#include "TimerManager.h"
#include "CharStatesManager.h"
#include "Engine/Engine.h"
#include "GameplayTagContainer.h"


void UCharActions::Enter(TSubclassOf <UCharStatesBase> LastState)
{
    Super::Enter(LastState);

    if(EnableAutoExitFunction)
    {
        AutoExitWrapper();
        
        GetWorld()->GetTimerManager().SetTimer
        (
            AutoExitTick,                                // O handle para controlar o timer
            this,                                        // O objeto que contém a função a ser chamada
            &UCharActions::AutoExitWrapper,             // A função a ser chamada
            AutoExit_Interval,                           // Intervalo
            true                                         // Se deve repetir
        );
    }
}

bool UCharActions::ActiveCondition_Implementation()
{
    return true;
}

bool UCharActions::VerifyAction()
{
    bool GroupIsValid = false;
    bool TagIsValid = false;
    bool BlockedTagsIsValid = false;
    
    FGameplayTag CSM_Group = CSM_Owner->ActiveGroup;
    FGameplayTagContainer CSM_Tags = CSM_Owner->TagStates;

    if(MyGroups.IsEmpty())
    {
        GroupIsValid = true;
    }
    else
    {
        GroupIsValid = MyGroups.HasTag(CSM_Group);
        
        if(!GroupIsValid)
        {
            return false;
        }

    }

    if(MyTags.IsEmpty())
    {
        TagIsValid = true;
    }
    else
    {
        TagIsValid = CSM_Tags.HasAll(MyTags);

        if(!TagIsValid)
        {
            return false;
        }
    }

    if(MyBlockedTags.IsEmpty())
    {
        BlockedTagsIsValid = true;
    }
    else
    {
        BlockedTagsIsValid = !CSM_Tags.HasAny(MyBlockedTags);

        if(!BlockedTagsIsValid)
        {
            return false;
        }
    }
    const bool CanStart = ActiveCondition();
    
    return CanStart && GroupIsValid && TagIsValid && BlockedTagsIsValid;
}


// FUNCTIONS TO USER
// Stop Action
void UCharActions::StopAction()
{
    CSM_Owner->StopAction();
}

// OVERRIDED Functions
// Cancel Action
void UCharActions::CancelAction(TSubclassOf <UCharStatesBase> NewAction)
{
    if(CallOnExitWhenCanceled)
    {
        // Se a função Exit estiver linkada com Canceled, chamamos ela.
        if(EnableAutoExitFunction)
        {
            // Se o Auto Exit estiver ativo, desligamos ele.
            GetWorld()->GetTimerManager().ClearTimer(AutoExitTick);
        }

        Exit(NewAction);

        OnCanceled(NewAction);
    }
    else
    {
        if(EnableTickUpdate)
        {
            // Se o Persistent Tick estiver ativo, desligamos ele.
            GetWorld()->GetTimerManager().ClearTimer(PersistentTick);
        }

        if(EnableAutoExitFunction)
        {
            // Se o Auto Exit estiver ativo, desligamos ele.
            GetWorld()->GetTimerManager().ClearTimer(AutoExitTick);
        }
    
        OnCanceled(NewAction);
    }

}

void UCharActions::OnCanceled_Implementation(TSubclassOf <UCharStatesBase> NewAction)
{}

// Auto Exit
void UCharActions::AutoExitWrapper()
{
    if(AutoExitVerifier())
    {
        if(EnableAutoExitFunction)
        {
            // Se o Auto Exit estiver ativo, desligamos ele.
            GetWorld()->GetTimerManager().ClearTimer(AutoExitTick);
        }
        
        StopAction();
    }
}

bool UCharActions::AutoExitVerifier_Implementation()
{return false;}

// Action Error
void UCharActions::OnActivateFail_Implementation(EActionErrorType ErrorType)
{}