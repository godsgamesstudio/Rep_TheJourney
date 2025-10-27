// Fill out your copyright notice in the Description page of Project Settings.


#include "CSM_PrimaryData.h"
#include "CharMasterState.h"
#include "CharSpecificState.h"
#include "CharActions.h"
#include "CharReactions.h"

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////
// GET Classes
TArray <TSubclassOf<UCharMasterState>> UCSM_PrimaryData::GetMasterStates()
{
    TArray <TSubclassOf<UCharMasterState>> AllMS;
    MasterStates.GenerateKeyArray(AllMS);

    if(AllMS.IsEmpty())
    {
        return {};
    }
    
    TArray <TSubclassOf <UCharMasterState>> SaveMS;

    for(TSubclassOf <UCharMasterState>& MS : AllMS)
    {
        if(MS)
        {
            SaveMS.Add(MS);
        }
    }

    if(SaveMS.IsEmpty())
    {
        return {};
    }

    return SaveMS;
}

TArray <TSubclassOf <UCharSpecificState>> UCSM_PrimaryData::GetSpecicicStates(TSubclassOf <UCharMasterState> MasterState)
{
    if(!MasterState)
    {
        return {};
    }
    const FStrc_MS_Setup* Infos = MasterStates.Find(MasterState);

    if(!Infos)
    {
        return {};
    }

    TArray <TSubclassOf<UCharSpecificState>> SpecificStates;
    Infos->SpecificStates.GenerateKeyArray(SpecificStates);

    if(SpecificStates.IsEmpty())
    {
        return {};
    }
    return SpecificStates;
}

TArray <TSubclassOf <UCharActions>> UCSM_PrimaryData::GetActions()
{
    TArray <TSubclassOf<UCharActions>> AllAct;
    Actions.GenerateKeyArray(AllAct);
    TArray <TSubclassOf<UCharActions>> Ok_AllAct;

    if(AllAct.IsEmpty())
    {
        return {};
    }

    for(int32 i = AllAct.Num() -1; i >= 0; --i)
    {
        if(AllAct[i])
        {
            Ok_AllAct.Add(AllAct[i]);
        }
    }

    if(Ok_AllAct.IsEmpty())
    {
        return {};
    }

    return Ok_AllAct;
}

TArray <TSubclassOf <UCharReactions>> UCSM_PrimaryData::GetReactions()
{
    TArray <TSubclassOf <UCharReactions>> AllRact;
    Reactions.GenerateKeyArray(AllRact);
    TArray <TSubclassOf <UCharReactions>> Ok_AllRact;

    if(AllRact.IsEmpty())
    {
        return {};
    }

    for(int32 i = AllRact.Num() -1; i >= 0; --i)
    {
        if(AllRact[i])
        {
            Ok_AllRact.Add(AllRact[i]);
        }
    }

    if(Ok_AllRact.IsEmpty())
    {
        return {};
    }

    return Ok_AllRact;
}

// Can Enter In States
TSet <TSubclassOf <UCharMasterState>> UCSM_PrimaryData::Get_MS_CanEnter(TSubclassOf <UCharMasterState> MasterState)
{
    if(MasterStates.Contains(MasterState))
    {
        const FStrc_MS_Setup* MS_Infos = MasterStates.Find(MasterState);

        if(MS_Infos)
        {
            TSet <TSubclassOf <UCharMasterState>> MS_Classes = MS_Infos->CanEnterIn;
            return MS_Classes;
        }
    }
    return {};
}

TSet <TSubclassOf <UCharSpecificState>> UCSM_PrimaryData::Get_SS_CanEnter(TSubclassOf <UCharMasterState> MasterState, TSubclassOf <UCharSpecificState> SpecificState)
{
    // Primeiro verificamos se o Master State existe
    if(MasterStates.Contains(MasterState))
    {
        FStrc_MS_Setup* MS_Infos = MasterStates.Find(MasterState);
        
        // Agora usando as informações do MS, verificamos se o Specific State existe dentro do MS informado.
        if(MS_Infos->SpecificStates.Contains(SpecificState))
        {
            FStrc_SS_Setup* SS_Infos = MS_Infos->SpecificStates.Find(SpecificState);

            const TSet <TSubclassOf <UCharSpecificState>> SS_Classes = SS_Infos->CanEnterIn;
            return SS_Classes;
        }
    }
    return {};
}

// Get Actions by SS
TSubclassOf <UCharActions> UCSM_PrimaryData::GetActionsBySS(TSubclassOf<UCharSpecificState> SpecificState, const UInputAction* ActionInput)
{
    for (const auto& Pair : Actions)
    {
        const TSubclassOf<UCharActions> ActionClass = Pair.Key;
      	const FStrc_Act_Setup& ActionConfig = Pair.Value;

        if(ActionConfig.CanUseIn.Contains(SpecificState) && ActionConfig.InputAction == ActionInput)
        {
            return ActionClass;
        }
    }

return nullptr;
}

// GET INFOS 

FStrc_MS_Setup UCSM_PrimaryData::GetMasterStateInfos(TSubclassOf <UCharMasterState> MasterState) const
{
    const FStrc_MS_Setup* MS_Infos = MasterStates.Find(MasterState);

    if(MS_Infos)
    {
        return *MS_Infos;
    }
    return FStrc_MS_Setup();

}

FStrc_SS_Setup UCSM_PrimaryData::GetSpecificStateInfos(TSubclassOf <UCharMasterState> MasterState, TSubclassOf <UCharSpecificState> SpecificState) const
{
    FStrc_MS_Setup MS_Info = GetMasterStateInfos(MasterState);
    
    const FStrc_SS_Setup* SS_Infos = MS_Info.SpecificStates.Find(SpecificState);

    if(SS_Infos)
    {
        return *SS_Infos;
    }
    return FStrc_SS_Setup();

}

FStrc_Act_Setup UCSM_PrimaryData::GetActionInfo(TSubclassOf <UCharActions> Action)
{
    if(Action)
    {
        FStrc_Act_Setup Info = Actions.FindRef(Action);

        return Info;
    }
    return FStrc_Act_Setup();
}

FStrc_Ract_Setup UCSM_PrimaryData::GetReactionInfo(TSubclassOf <UCharReactions> Reaction)
{
    if(Reaction)
    {
        FStrc_Ract_Setup Info = Reactions.FindRef(Reaction);

        return Info;
    }
    return FStrc_Ract_Setup();
}
