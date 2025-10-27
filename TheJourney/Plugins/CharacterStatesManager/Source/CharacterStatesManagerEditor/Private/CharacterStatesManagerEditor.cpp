#include "CharacterStatesManagerEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"



// (Opcional, mas boa prática) Define um namespace de texto para este arquivo, para evitar colisões
#define LOCTEXT_NAMESPACE "FCharacterStatesManagerEditorModule"

void FCharacterStatesManagerEditorModule::StartupModule()
{
    // 4. Carrega o módulo "AssetTools" do motor. É ele que gerencia o registro de novos tipos de asset.
    
}

void FCharacterStatesManagerEditorModule::ShutdownModule()
{
    // Lógica de limpeza, se necessário

    // Verifica se o módulo AssetTools ainda está carregado antes de usá-lo
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {

    }
    
}

IMPLEMENT_MODULE(FCharacterStatesManagerEditorModule, CharacterStatesManagerEditor);

#undef LOCTEXT_NAMESPACE
