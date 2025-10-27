// Fill out your copyright notice in the Description page of Project Settings.


#include "CharMasterState.h"
#include "CharStatesManager.h"
#include "GameplayTagContainer.h"

bool UCharMasterState::VerifyMasterState()
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
    const bool CanStart = StartCondition();
    
    return CanStart && GroupIsValid && TagIsValid && BlockedTagsIsValid;
}

bool UCharMasterState::StartCondition_Implementation()
{
    return true;
}