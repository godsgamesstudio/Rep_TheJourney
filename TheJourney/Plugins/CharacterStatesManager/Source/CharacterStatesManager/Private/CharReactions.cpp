// Fill out your copyright notice in the Description page of Project Settings.


#include "CharReactions.h"
#include "GameplayTagContainer.h"
#include "CharStatesManager.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

// Active Tick and Timers
UWorld* UCharReactions::GetWorld() const
{
    if(CSM_Owner)
    {
        return CSM_Owner->GetWorld();
    }
    return nullptr;
}

// FUNCTIONS _______________________________________________
void UCharReactions::Initialize (class APawn* Pawn_Ref, class UCharStatesManager* CSM_Ref, FGameplayTagContainer Groups, FGameplayTagContainer Tags, FGameplayTagContainer BlockedTags)
{
    Pawn_Owner = Pawn_Ref;
    CSM_Owner = CSM_Ref;

    MyGroups = Groups;
    MyTags = Tags;
    MyBlockedTags = BlockedTags;
}

bool UCharReactions::VerifyReaction()
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
    const bool CanStart = ConditionToEnter();
    
    return CanStart && GroupIsValid && TagIsValid && BlockedTagsIsValid;
}

void UCharReactions::Enter(class UObject* ActorCauser, FGameplayTagContainer PayloadTags)
{
    OnEnter(ActorCauser, PayloadTags);

    if(EnableTickUpdate)
    {
        CallPersistentTick();
        
        GetWorld()->GetTimerManager().SetTimer
        (
            PersistentTick,                              // O handle para controlar o timer
            this,                                        // O objeto que contém a função a ser chamada
            &UCharReactions::CallPersistentTick,         // A função a ser chamada
            PersistentTick_Interval,                     // Intervalo
            true                                         // Se deve repetir
        );
    }
}

void UCharReactions::CallPersistentTick()
{
    OnPersistentTick(PersistentTick_Interval);
}

void UCharReactions::CancelReaction(TSubclassOf <UCharReactions> NewReaction)
{
    OnCanceled(NewReaction);

    StopReaction();
}

bool UCharReactions::GetCanMovement()
{
    return EnableCharMovement;
}

// FUNCTIONS TO USER USE
void UCharReactions::StopReaction()
{
    if(EnableTickUpdate)
    {
        // Se o Persistent Tick estiver ativo, desligamos ele.
        GetWorld()->GetTimerManager().ClearTimer(PersistentTick);
    }

    CSM_Owner->StopReaction();
}

// FUNCTIONS TO OVERRIDE ____________________________________
bool UCharReactions::ConditionToEnter_Implementation()
{return true;}

void UCharReactions::OnEnter_Implementation(class UObject* ActorCauser, FGameplayTagContainer PayloadTags)
{}

void UCharReactions::OnPersistentTick_Implementation(float IntervalTime)
{}

void UCharReactions::OnCanceled_Implementation(TSubclassOf <UCharReactions> NewReaction)
{}
