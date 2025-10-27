// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStatesBase.h"
#include "GameplayTagcontainer.h"
#include "GameplayTagsManager.h"
#include "TimerManager.h"
#include "CharStatesManager.h"
#include "CharReactions.h"


void UCharStatesBase::Initialize (class APawn* Pawn_Ref, class UCharStatesManager* CSM_Ref, FGameplayTagContainer Groups, FGameplayTagContainer Tags, FGameplayTagContainer BlockedTags)
{
    if (Pawn_Ref && CSM_Ref)
    {
        Pawn_Owner = Pawn_Ref;
        CSM_Owner = CSM_Ref;

        // ADD Dynamic binds to Manager
        CSM_Owner->OnMasterStateChanged.AddDynamic(this, &UCharStatesBase::OnManagerChangedMasterState);
        CSM_Owner->OnSpecificStateChanged.AddDynamic(this, &UCharStatesBase::OnManagerChangedSpecificState);
        CSM_Owner->OnActionEntry.AddDynamic(this, &UCharStatesBase::OnManagerActionEntry);
        CSM_Owner->OnActionExit.AddDynamic(this, &UCharStatesBase::OnManagerActionExit);
        CSM_Owner->OnLoadingCompleted.AddDynamic(this, &UCharStatesBase::OnBeginPlay);

        // Set Groups & Tags

        MyGroups = Groups;
        MyTags = Tags;
        MyBlockedTags = BlockedTags;
    }
}

UWorld* UCharStatesBase::GetWorld() const
{
    if(CSM_Owner)
    {
        return CSM_Owner->GetWorld();
    }
    return nullptr;
}

void UCharStatesBase::Enter(TSubclassOf <UCharStatesBase> LastState)
{
    OnEnter(LastState);

    // Se o Persistent Tick estiver ativo.
    if(EnableTickUpdate)
    {
        CallPersistentTick();
        
        GetWorld()->GetTimerManager().SetTimer
        (
            PersistentTick,                              // O handle para controlar o timer
            this,                                        // O objeto que contém a função a ser chamada
            &UCharStatesBase::CallPersistentTick,          // A função a ser chamada
            PersistentTick_Interval,                     // Intervalo
            true                                         // Se deve repetir
        );
    }
}

void UCharStatesBase::CallPersistentTick()
{
    if(!CSM_Owner->GetCurrentReaction())
    {
        OnPersistentTick(PersistentTick_Interval);
    }
}

void UCharStatesBase::Exit(TSubclassOf <UCharStatesBase> NewState)
{
    if(EnableTickUpdate)
    {
        // Se o Persistent Tick estiver ativo, desligamos ele.
        GetWorld()->GetTimerManager().ClearTimer(PersistentTick);
    }
    
    OnExit(NewState);
}

void UCharStatesBase::OnEnter_Implementation(TSubclassOf <UCharStatesBase> LastState)
{}

void UCharStatesBase::OnExit_Implementation(TSubclassOf <UCharStatesBase> NewState)
{}

void UCharStatesBase::OnPersistentTick_Implementation(float IntervalTime)
{}

// Functions to User
TSubclassOf <UCharMasterState> UCharStatesBase::GetCurrentMasterState() const
{
    return CSM_Owner->GetCurrentMasterState();
}

TSubclassOf <UCharSpecificState> UCharStatesBase::GetCurrentSpecificState() const
{
    return CSM_Owner->GetCurrentSpecificState();
}

// Manager BINDS
void UCharStatesBase::OnManagerChangedMasterState_Implementation(TSubclassOf <UCharMasterState> NewMasterState, TSubclassOf <UCharMasterState> OldMasterState)
{}

void UCharStatesBase::OnManagerChangedSpecificState_Implementation(TSubclassOf <UCharSpecificState> NewSpecificState, TSubclassOf <UCharSpecificState> OldSpecificState)
{}

void UCharStatesBase::OnManagerActionEntry_Implementation(TSubclassOf <UCharActions> Action)
{}

void UCharStatesBase::OnManagerActionExit_Implementation(TSubclassOf <UCharActions> Action)
{}

void UCharStatesBase::OnBeginPlay_Implementation()
{}