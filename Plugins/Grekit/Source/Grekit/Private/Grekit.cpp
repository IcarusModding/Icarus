#include "Grekit.h"
#include "GrekitStyle.h"
#include "GrekitCommands.h"
#include "GrekitWidget.h"
#include "AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "LevelEditor.h"

static const FName GrekitTabName("Grekit");

#define LOCTEXT_NAMESPACE "FGrekitModule"

DEFINE_LOG_CATEGORY(LogGrekit)

void FGrekitModule::StartupModule() {
    //Register editor settings
    RegisterSettings();

    //Initialize Slate stuff, including commands
    FGrekitStyle::Initialize();
    FGrekitStyle::ReloadTextures();
    FGrekitCommands::Register();
    
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FGrekitCommands::Get().OpenPluginWindow,
        FExecuteAction::CreateLambda([](){
            FGlobalTabmanager::Get()->InvokeTab(GrekitTabName);
        }),
        FCanExecuteAction());
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
    
    //Register Grekit Settings in Editor's Project Menu
    TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
    MenuExtender->AddMenuExtension(TEXT("FileProject"), EExtensionHook::After, PluginCommands,
        FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& Builder){
            Builder.AddMenuEntry(FGrekitCommands::Get().OpenPluginWindow);
        }));
    LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
    
    //Register Grekit Settings in Editor's Toolbar
    TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
    ToolbarExtender->AddToolBarExtension(TEXT("Settings"), EExtensionHook::After, PluginCommands,
        FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& Builder) {
            Builder.AddToolBarButton(FGrekitCommands::Get().OpenPluginWindow);
        }));
    LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

    //Register normal grekit settings tab spawner
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GrekitTabName,
        FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&){
            const TSharedRef<SGrekitWidget> GrekitWidget = SNew(SGrekitWidget);
            return SNew(SDockTab)
                .TabRole(NomadTab)
                // TODO: Save settings when tab is closed automatically
                //.OnTabClosed_Lambda([GrekitWidget](TSharedRef<SDockTab>) { GrekitWidget->SaveSettings(); })
                [ GrekitWidget ];
        }))
        .SetDisplayName(LOCTEXT("FGrekitTabTitle", "Grekit"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGrekitModule::ShutdownModule() {
    if (UObjectInitialized()) {
        UnregisterSettings();
    }
    
    FGrekitStyle::Shutdown();
    FGrekitCommands::Unregister();

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GrekitTabName);
}

void FGrekitModule::RegisterSettings() const {
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"))) {
        // Create the new category
        ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer(TEXT("Project"));
        SettingsContainer->DescribeCategory(TEXT("Grekit Settings"),
            LOCTEXT("RuntimeWDCategoryName", "Grekit Settings"),
            LOCTEXT("RuntimeWDCategoryDescription", "Grekit Settings. Can also be edited in the Grekit menu"));

        // Register the settings
        ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Grekit"), TEXT("General"),
            LOCTEXT("RuntimeGeneralSettingsName", "General"),
            LOCTEXT("RuntimeGeneralSettingsDescription", "Grekit Settings. Can also be edited in the Grekit menu"),
            UGrekitSettings::Get()
        );
        // Register the save handler to your settings, you might want to use it to
        // validate those or just act to settings changes.
        if (SettingsSection.IsValid()) {
            SettingsSection->OnModified().BindLambda([](){
                UGrekitSettings::Get()->SaveSettings();
                return true;
            });
        }
    }
}

void FGrekitModule::UnregisterSettings() const {
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"))) {
        SettingsModule->UnregisterSettings(TEXT("Project"), TEXT("Grekit"), TEXT("General"));
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGrekitModule, Grekit)