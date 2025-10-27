#pragma once
#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleInterface.h"

// Forward declare a classe IAssetTypeActions para evitar includes desnecessários no .h
class IAssetTypeActions;

class FCharacterStatesManagerEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    // Variável para guardar o ID da nossa categoria customizada
    EAssetTypeCategories::Type MyAssetCategory;

    // Lista para guardar ponteiros para nossas classes de ação registradas,
    // para que possamos cancelar o registro delas no ShutdownModule.
    TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetTypeActions;
};
