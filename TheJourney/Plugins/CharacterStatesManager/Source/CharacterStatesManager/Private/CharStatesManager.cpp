// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStatesManager.h"
#include "GameFramework/Pawn.h"
#include "CSM_PrimaryData.h"
#include "CharMasterState.h"
#include "CharSpecificState.h"
#include "CharActions.h"
#include "CharReactions.h"
#include "Engine/Engine.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"


// Sets default values for this component's properties
UCharStatesManager::UCharStatesManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false; // Faz o componente startar com o tick desligado.

	// ...
}


// Called when the game starts
void UCharStatesManager::BeginPlay()
{
	Super::BeginPlay();

	// Variáveis Local
	TArray <TSubclassOf <UCharSpecificState>> LSS_Classes; // Uma Array que será usada na geração dos Specific States
	
	Pawn_Owner = Cast <APawn> (GetOwner()); // Faz um cast do Owner para Pawn e salva na variável
	
	// Este if verifica se o Owner é realmente um Pawn.
	if (!Pawn_Owner)
	{
		BeginPlayError(TEXT("CSM | The Owner is not a Pawn or Character."));
		return;
	}
	
    // Agora vamos verificar se os Profiles estão vazios. 
	if(Profiles.IsEmpty())
	{
		BeginPlayError(TEXT("CSM | no profiles were found on State Manager"));
	    return;
	}
    
	// Não estando vazio, agora vamos verificar a existencia de assets válidos e fazemos o primeiro Setup do sistema
	for(const TPair <FGameplayTag, UCSM_PrimaryData*>& Pair : Profiles)
	{
		// Por estarmos no Begin Play, primeiro verificamos se há um Data Asset atual, se não, sigfica que precisamos adicionar e que é a primeira iteração
		// Se já existi um valor de Data Asset, significa que já passamos da primeira iteração, então as próximas vamos apenas verificar os "null" e emitir avisos.
		if(!CSM_DataAsset)
		{
			if(Pair.Value)
			{
				CSM_ProfileName = Pair.Key;
				CSM_DataAsset = Pair.Value;
			}
			else
			{
				BeginPlayError(TEXT("CSM | no profiles were found on State Manager"));
				return;
			}			
		}
		else
		{
			if(!Pair.Value)
			{
				BeginPlayError(TEXT("CSM | Found a Profile with a null Data Asset."));
			}
		}
	}

	// Criação dos ESTADOS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(const TPair <FGameplayTag, UCSM_PrimaryData*>& Pair : Profiles)
	{
		TArray<TSubclassOf<UCharMasterState>> LMS_Classes = Pair.Value->GetMasterStates();

		if(LMS_Classes.IsEmpty()) // Verificamos se existe Master States configurados
		{	
			BeginPlayError(TEXT("CSM | No Master States found."));
			return;
		}

		for(TSubclassOf<UCharMasterState>& LMS_ClasseI : LMS_Classes)
		{
			if(LMS_ClasseI)
			{
				// Geração dos Estados Mestres ----------------------------------------------------
				
				// A instância só é criada, caso não exista ainda na "biblioteca" de Master States
				if(!MS_Objs.Contains(LMS_ClasseI))
				{
					// Cria e salva a instancia
					TObjectPtr <UCharMasterState> MS_Obj = NewObject <UCharMasterState>(this, LMS_ClasseI);

					// Obtém as tags e grupos e salva as informações
					FGameplayTagContainer MSGroups = Pair.Value->GetMasterStateInfos(LMS_ClasseI).Groups;
					FGameplayTagContainer MSTags = Pair.Value->GetMasterStateInfos(LMS_ClasseI).RequiredTags;
					FGameplayTagContainer MSBlockedTags = Pair.Value->GetMasterStateInfos(LMS_ClasseI).BlockedTags;
			
					// Inicializa a instancia passando os valores necessários
					MS_Obj->Initialize(Pawn_Owner, this, MSGroups, MSTags, MSBlockedTags);

					// Adiciona as referências na "biblioteca" de Master States do CSM
					MS_Objs.Add(LMS_ClasseI, MS_Obj);
				}

				// Geração dos Estados Específicos ------------------------------------------------

				LSS_Classes = Pair.Value->GetSpecicicStates(LMS_ClasseI);
				LSS_Classes.Remove(nullptr);

				if(LSS_Classes.IsEmpty())
				{
					BeginPlayError(TEXT("Some Master States do not have Specific States."));
				 	return;
				}

				for(TSubclassOf <UCharSpecificState>& LSS_ClassesI : LSS_Classes) // Neste loop vamos verificar se cada elemento está preenchido, se não, iremos retirar da array.
				{
					if(LSS_ClassesI)
					{
						// Se o Estado Específico não existir na biblioteca de Specific States, ele então é criado.
						if(!SS_Objs.Contains(LSS_ClassesI))
						{
							//Cria e inicializa o asset de Estado
							TObjectPtr <UCharSpecificState> SS_Obj = NewObject <UCharSpecificState>(this, LSS_ClassesI);
							FGameplayTagContainer SSGroups = Pair.Value->GetSpecificStateInfos(LMS_ClasseI, LSS_ClassesI).Groups;
							FGameplayTagContainer SSTags = Pair.Value->GetSpecificStateInfos(LMS_ClasseI, LSS_ClassesI).RequiredTags;
							FGameplayTagContainer SSBlockedTags = Pair.Value->GetSpecificStateInfos(LMS_ClasseI, LSS_ClassesI).BlockedTags;

							SS_Obj->Initialize(Pawn_Owner, this, SSGroups, SSTags, SSBlockedTags);

							SS_Objs.Add(LSS_ClassesI, SS_Obj);
						}
					}   
				}
			}
		}	

		// Criação das AÇÕES //////////////////////////////////////////////////////////////////////////////////////
		TArray <TSubclassOf<UCharActions>> LAct_Classes = Pair.Value->GetActions();

		if(!LAct_Classes.IsEmpty())
		{	
			for(TSubclassOf <UCharActions>& LAct_ClassesI : LAct_Classes)
			{
				// Cria e inicializa o asset de Ação
				if(LAct_ClassesI)
				{
					if(!Act_Objs.Contains(LAct_ClassesI))
					{
						TObjectPtr <UCharActions> Act_Obj = NewObject <UCharActions> (this, LAct_ClassesI);

						FGameplayTagContainer Groups = Pair.Value->GetActionInfo(LAct_ClassesI).Groups;
						FGameplayTagContainer Tags = Pair.Value->GetActionInfo(LAct_ClassesI).RequiredTags;
						FGameplayTagContainer BlockedTags = Pair.Value->GetActionInfo(LAct_ClassesI).BlockedTags;

						Act_Obj->Initialize(Pawn_Owner, this, Groups, Tags, BlockedTags);
			
						Act_Objs.Add(LAct_ClassesI, Act_Obj);
					}
				}
			}
		}

		// Criação das REAÇÕES ////////////////////////////////////////////////////////////////////////////////////////
		TArray <TSubclassOf <UCharReactions>> LRact_Classes = Pair.Value->GetReactions();

		if(!LRact_Classes.IsEmpty())
		{
			for(TSubclassOf <UCharReactions>& LRact_ClasseI : LRact_Classes)
			{
				if(LRact_ClasseI)
				{
					if(!Ract_Objs.Contains(LRact_ClasseI))
					{
						TObjectPtr <UCharReactions> Ract_Obj = NewObject <UCharReactions> (this, LRact_ClasseI);

						FGameplayTagContainer Groups = Pair.Value->GetReactionInfo(LRact_ClasseI).Groups;
						FGameplayTagContainer Tags = Pair.Value->GetReactionInfo(LRact_ClasseI).RequiredTags;
						FGameplayTagContainer BlockedTags = Pair.Value->GetReactionInfo(LRact_ClasseI).BlockedTags;

						Ract_Obj->Initialize(Pawn_Owner, this, Groups, Tags, BlockedTags);

						Ract_Objs.Add(LRact_ClasseI, Ract_Obj);
					}
				}
			}
		}
	}

	// Configuração de Grupos e Tags Iniciais
	ActiveGroup = CSM_DataAsset->DefaultActiveGroup;
	TagStates = CSM_DataAsset->DefaultTagStates;

	// Save All Classes
	MS_Objs.GenerateKeyArray(MS_Classes);
	SS_Objs.GenerateKeyArray(SS_Classes);
	Act_Objs.GenerateKeyArray(Act_Classes);	
	Ract_Objs.GenerateKeyArray(Ract_Classes);

	// CALL Dispatcher On CSM Load
	OnLoadingCompleted.Broadcast();

	// Vamos ativar as verificações de estados
	SetComponentTickEnabled(true);
}


// Called every frame
void UCharStatesManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(ComponentDisabled)
	{
		// Caso o componente esteja "desabilitado" será enviado uma mensagem no PRINTSTRING com o erro.
		GEngine-> AddOnScreenDebugMessage(-1, -1.f, FColor::Red, ERROR_Text); 
		
	}
	else
	{
		// Caso o componente esteja "habilitado" faremos as verificações de estados.

		// Verificação de Estados Mestres ///////////////////////////
		if(Current_MS)
		{
			// Se tiver um MS Atual, verificamos os que ele permite a entrada
			const TSet <TSubclassOf <UCharMasterState>> CanVerify = CSM_DataAsset->Get_MS_CanEnter(Current_MS);

			for(const TSubclassOf <UCharMasterState>& MS : CanVerify)
			{
				const TObjectPtr <UCharMasterState> NewMS_Ref = MS_Objs.FindRef(MS);

				if(NewMS_Ref)
				{
					const bool CanEnter = NewMS_Ref->VerifyMasterState();

					if(CanEnter)
					{
						// Precisamos também remover o Specific State Atual
						if(Current_SS)
						{
							TObjectPtr <UCharSpecificState> SS_Ref = SS_Objs.FindRef(Current_SS);							
							SS_Ref->Exit(nullptr);
							TSubclassOf <UCharSpecificState> Save_SS = Current_SS;
							Current_SS = nullptr;

							OnSpecificStateChanged.Broadcast(nullptr, Save_SS);
						}
						
						// Primeiro vamos obter os nomes dos estados que vão entrar e sair.
						const TObjectPtr <UCharMasterState> CurrentMS_Ref = MS_Objs.FindRef(Current_MS);

						// Agora podemos chamar respectivamente os eventos de saída e entrada dos estados.						
						CurrentMS_Ref->Exit(Current_MS);
						NewMS_Ref->Enter(MS);
						TSubclassOf <UCharMasterState> Save_MS = Current_MS;
						Current_MS = MS;						

						// Chama o Dispatcher On Master State Changed
						OnMasterStateChanged.Broadcast(Current_MS, Save_MS);
					}
				}
			}
		}
		else
		{
			// Se não tiver um MS Atual, verificamos todos que existir
			for(TSubclassOf <UCharMasterState>& MS : MS_Classes)
			{
				const TObjectPtr <UCharMasterState> MS_Ref = MS_Objs.FindRef(MS);
				
				if(MS_Ref)
				{
					const bool CanEnter = MS_Ref->VerifyMasterState();

					if(CanEnter)
					{
						// Precisamos também remover o Specific State Atual
						if(Current_SS)
						{
							TObjectPtr <UCharSpecificState> SS_Ref = SS_Objs.FindRef(Current_SS);
							SS_Ref->Exit(nullptr);
							TSubclassOf <UCharSpecificState> Save_SS = Current_SS;
							Current_SS = nullptr;

							OnSpecificStateChanged.Broadcast(nullptr, Save_SS);
						}
						
						MS_Ref->Enter(nullptr);
						Current_MS = MS;

						// Chama o Dispatcher On Master State Changed
						OnMasterStateChanged.Broadcast (Current_MS, nullptr);
					}
				}
			}
		}

		// Verificação de Estados Específicos /////////////////////////////

		if(Current_MS)
		{
			if(Current_SS)
			{
				// Se tiver um atual, vamos verificar com base na disponibilidade do estado específico.
				const TSet <TSubclassOf <UCharSpecificState>> CanVerify = CSM_DataAsset->Get_SS_CanEnter(Current_MS, Current_SS);

				for(const TSubclassOf <UCharSpecificState>& SS : CanVerify)
				{
					const TObjectPtr <UCharSpecificState> NewSS_Ref = SS_Objs.FindRef(SS);

					if(NewSS_Ref)
					{
						const bool CanEnter = NewSS_Ref->VerifySpecificState();

						if(CanEnter)
						{
							const TObjectPtr <UCharSpecificState> CurrentSS_Ref = SS_Objs.FindRef(Current_SS);
							
							CurrentSS_Ref->Exit(Current_SS);
							NewSS_Ref->Enter(SS);
							TSubclassOf <UCharSpecificState> Save_SS = Current_SS;
							Current_SS = SS;

							// Chama o Dispatcher On Specific State Changed
							OnSpecificStateChanged.Broadcast(Current_SS, Save_SS);
						}
					}
				}
			}
			else
			{
				// Se não tiver um atual, vamos verificar TODOS que o Master State permite.
				const TArray <TSubclassOf <UCharSpecificState>> CanVerify = CSM_DataAsset->GetSpecicicStates(Current_MS);

				for(const TSubclassOf <UCharSpecificState>& SS : CanVerify)
				{
					const TObjectPtr <UCharSpecificState> SS_Ref = SS_Objs.FindRef(SS);

					if(SS_Ref)
					{
						const bool CanEnter = SS_Ref->VerifySpecificState();

						if(CanEnter)
						{							
							SS_Ref->Enter(nullptr);
							Current_SS = SS;

							// Chama o Dispatcher On Specific State Changed
							OnSpecificStateChanged.Broadcast(SS, nullptr);
						}
					}
				}
			}
	    }
	}
}

////// FUNCTIONS

// Begin Play Error
void UCharStatesManager::BeginPlayError(FString Text)
{
	// Desativamos o componente e interrompemos o Begin play
	ERROR_Text = Text;
	ComponentDisabled = true;
	SetComponentTickEnabled(true);
}

// System Functions
void UCharStatesManager::StopAction()
{
	if(Current_Act)
	{
		TSubclassOf <UCharActions> SaveAct = Current_Act;

		Current_Act = nullptr;
		Act_Objs.FindRef(SaveAct)->Exit(nullptr);

		OnActionExit.Broadcast(SaveAct);
	}
}

void UCharStatesManager::StopReaction()
{
	if(Current_Ract)
	{
		TSubclassOf <UCharReactions> SaveRact = Current_Ract;

		Current_Ract = nullptr;
	}
}

void UCharStatesManager::ExitAllStates()
{
	if(Current_MS)
	{
		TSubclassOf <UCharMasterState> SaveMS = Current_MS;
		MS_Objs.FindRef(Current_MS)->Exit(nullptr);
		Current_MS = nullptr;

		OnMasterStateChanged.Broadcast(nullptr, SaveMS);

		if(Current_SS)
		{
			TSubclassOf <UCharSpecificState> SaveSS = Current_SS;
			SS_Objs.FindRef(Current_SS)->Exit(nullptr);
			Current_SS = nullptr;

			OnSpecificStateChanged.Broadcast(nullptr, SaveSS);
		}
	}
}

// Functions to User
bool UCharStatesManager::PlayAction(UInputAction* ActionInput)
{

	// Se não conter nada na entrada de Input Action, retornamos valor falso
	if(!ActionInput)
	{
		return false;
	}	

	// Agora iremos verificar todas as ações para encontrar qual usa o Input Action
	for(const TSubclassOf <UCharActions>& Act : CSM_DataAsset->GetActions())
	{
		if(Act)
		{
			const UInputAction* IAction = CSM_DataAsset->Actions.Find(Act)->InputAction;

			if(IAction)
			{
				if(IAction == ActionInput)
				{
					FStrc_Act_Setup Act_Infos = CSM_DataAsset->GetActionInfo(Act);
					
					// Encontrado a ação correta, vamos verificar se a ação pode ser usada no Estado Específico Atual
					if(Act_Infos.CanUseIn.Contains(Current_SS))
					{
						// Checkamos se as verificações internas são verdadeiras
						TObjectPtr <UCharActions> NewAct_Obj = Act_Objs.FindRef(Act);
						
						if(NewAct_Obj)
						{
							if(NewAct_Obj->VerifyAction())
							{
								if(!Current_Act)
								{
									// Não tem ação - Vamos executar a ação								
									Current_Act = Act;
									NewAct_Obj->Enter(nullptr);
								
									OnActionEntry.Broadcast(Act);

									break;
								}
								else
								{
									// Tem ação - Vamos verificar se pode ser cancelada
									if(Act_Infos.CanInterrupt.Contains(Current_Act))
									{
										// Pode cancelar - Cancelamos a ação atual e depois executamos a nova ação

										// Cancelamento:									
										TSubclassOf <UCharActions> SaveAct = Current_Act;
										Current_Act = nullptr;
										Act_Objs.FindRef(SaveAct)->CancelAction(Act);

										OnActionExit.Broadcast(SaveAct);

										// Executar nova ação:									
										Current_Act = Act;
										NewAct_Obj->Enter(SaveAct);									

										OnActionEntry.Broadcast(Current_Act);

										break;
									}
									else
									{
										// Da erro por não conseguir cancelar outra ação
										NewAct_Obj->OnActivateFail(EActionErrorType::AER_OtherAction);
									}
								}
							}
							else
							{
								// Da erro por não tem sua condição
								NewAct_Obj->OnActivateFail(EActionErrorType::AER_ConditionFail);
							}
						}
					}
				}
			}
		}
	}	
	return false;
}

bool UCharStatesManager::PlayReaction(TSubclassOf <UCharReactions> Reaction, class UObject* CauserActor, FGameplayTagContainer PayloadTags)
{
	if(!CauserActor || !Reaction)
	{
		return false;
	}

	if(Ract_Classes.Contains(Reaction))
	{
		if(TObjectPtr <UCharReactions> Ract_Obj = Ract_Objs.FindRef(Reaction))
		{
			if(Ract_Obj->VerifyReaction())
			{
				if(Current_Ract)
				{
					if(CSM_DataAsset->GetReactionInfo(Reaction).CanInterrupt.Contains(Current_Ract))
					{
						Ract_Objs.FindRef(Current_Ract)->CancelReaction(Reaction);
						Current_Ract = nullptr;

						ExitAllStates();

						Ract_Obj->Enter(CauserActor, PayloadTags);
						Current_Ract = Reaction;

						return true;
					}
					return false;
				}
				else
				{
					if(Current_Act)
					{
						Act_Objs.FindRef(Current_Act)->CancelAction(nullptr);
						Current_Act = nullptr;
					}

					ExitAllStates();
				
					Ract_Obj->Enter(CauserActor, PayloadTags);
					Current_Ract = Reaction;

					return true;
				}
			}
		}
	}
	return false;
}

bool UCharStatesManager::ReleaseButton()
{
	if(Current_Act)
	{
		if(Act_Objs.FindRef(Current_Act)->EndOnInputRelease)
		{
			StopAction();
			return true;
		}
	}
	return false;
}

bool UCharStatesManager::CanCharacterMove()
{
	bool CanSS;
	bool CanRact;
	
	// Verify Specific State
	if(Current_SS)
	{
		const auto& SS = SS_Objs.FindRef(Current_SS);

		if(SS)
		{
			CanSS = SS->GetCanCharacterMove();
		}
		else
		{
			CanSS = true;
		}
	}
	else
	{
		CanSS = true;
	}

	// Verify Reaction
	if(Current_Ract)
	{
		const auto& Ract = Ract_Objs.FindRef(Current_Ract);

		if(Ract)
		{
			CanRact = Ract->GetCanMovement();
		}
		else
		{
			CanRact = true;
		}
	}
	else
	{
		CanRact = true;
	}

	// Generate Output
	return CanSS && CanRact;
}

bool UCharStatesManager::SetActiveGroup(FGameplayTag GroupTag)
{
	if(ActiveGroup == GroupTag)
	{
		return false;
	}

	ActiveGroup = GroupTag;
	
	Current_MS = nullptr;
	Current_SS = nullptr;
	
	return true;
}

bool UCharStatesManager::SetActiveTag(FGameplayTag Tag, bool Active)
{
	if(Tag.IsValid())
	{
		if(Active)
		{
			TagStates.AddTag(Tag);
		}
		else
		{
			TagStates.RemoveTag(Tag);
		}
		Current_MS = nullptr;
		Current_SS = nullptr;
		
		return true;
	}
	return false;
}

void UCharStatesManager::SetTagStates(FGameplayTagContainer TagContainer)
{
	TagStates = TagContainer;
}

FCSM_SaveInfos UCharStatesManager::GetSaveInfos() const
{
	FCSM_SaveInfos SaveInfos;

	SaveInfos.ActiveGroup = ActiveGroup;
	SaveInfos.ActiveTags = TagStates;

	return SaveInfos;
}

void UCharStatesManager::SetSaveInfos(FCSM_SaveInfos SaveInfos)
{
	ActiveGroup = SaveInfos.ActiveGroup;
	TagStates = SaveInfos.ActiveTags;
}

bool UCharStatesManager::SwitchProfile(FGameplayTag Profile)
{
	if(!Current_SS)
	{
		return false;
	}
	
	if(GetSpecificStateInstance(Current_SS)->GetCanSwitchProfile())
	{
		if(!CanSwitchProfileInActionOrReaction)
		{
			if(Current_Act || Current_Ract)
			{
				return false;
			}
		}

		if(Profiles.Contains(Profile))
		{
			FGameplayTag SaveProfile = CSM_ProfileName;
			
			TObjectPtr <UCSM_PrimaryData> NewDataAsset = Profiles.FindRef(Profile);
			
			if(NewDataAsset)
			{
				CSM_ProfileName = Profile;
				CSM_DataAsset = NewDataAsset;
				
				ExitAllStates();

				OnProfileChanged.Broadcast(Profile, SaveProfile);

				return true;
			}
		}
	}
	
	return false;
}


// PURE FUNCTIONS to User
TSubclassOf <UCharMasterState> UCharStatesManager::GetCurrentMasterState() const
{
	return Current_MS;
}

TSubclassOf <UCharSpecificState> UCharStatesManager::GetCurrentSpecificState() const
{
	return Current_SS;
}

TSubclassOf <UCharActions> UCharStatesManager::GetCurrentAction() const
{
	return Current_Act;
}

TSubclassOf <UCharReactions> UCharStatesManager::GetCurrentReaction() const
{
	return Current_Ract;
}


UCharMasterState* UCharStatesManager::GetMasterStateInstance(TSubclassOf <UCharMasterState> MasterState) const
{
	return MS_Objs.FindRef(MasterState);
}

UCharSpecificState* UCharStatesManager::GetSpecificStateInstance(TSubclassOf <UCharSpecificState> SpecificState) const
{
	return SS_Objs.FindRef(SpecificState);
}

UCharActions* UCharStatesManager::GetActionInstance(TSubclassOf <UCharActions> Action) const
{
	return Act_Objs.FindRef(Action);
}

UCharReactions* UCharStatesManager::GetReactionInstance(TSubclassOf <UCharReactions> Reaction) const
{
	return Ract_Objs.FindRef(Reaction);
}

FGameplayTag UCharStatesManager::GetCurrentGroup() const
{
	return ActiveGroup;
}

FGameplayTagContainer UCharStatesManager::GetCurrentTags() const
{
	return TagStates;
}