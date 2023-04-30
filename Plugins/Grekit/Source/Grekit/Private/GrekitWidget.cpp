#include "GrekitWidget.h"
#include "Grekit.h"
#include "GrekitModEntryList.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"

#define LOCTEXT_NAMESPACE "GrekitWidget"

void SGrekitWidget::Construct(const FArguments& InArgs) {
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bAllowSearch = true;
    DetailsViewArgs.bHideSelectionTip = true;
    DetailsViewArgs.bLockable = false;
    DetailsViewArgs.bSearchInitialKeyFocus = true;
    DetailsViewArgs.bUpdatesFromSelection = false;
    DetailsViewArgs.bShowOptions = true;
    DetailsViewArgs.bShowModifiedPropertiesOption = true;
    DetailsViewArgs.bShowActorLabel = false;
    DetailsViewArgs.bCustomNameAreaLocation = true;
    DetailsViewArgs.bCustomFilterAreaLocation = true;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.bAllowMultipleTopLevelObjects = true;
    DetailsViewArgs.bShowScrollBar = false;

    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    DetailsView->SetObject(UGrekitSettings::Get());
    DetailsView->OnFinishedChangingProperties().AddLambda([this](const FPropertyChangedEvent&) {
        UGrekitSettings::Get()->SaveSettings();
    });
    
    ChildSlot[
        SNew(SVerticalBox)
        +SVerticalBox::Slot().AutoHeight()[
            DetailsView.ToSharedRef()
        ]
        +SVerticalBox::Slot().AutoHeight()[
            SNew(SHorizontalBox)
            +SHorizontalBox::Slot().FillWidth(1)[
                SNew(SEditableTextBox)
                .HintText(LOCTEXT("SearchHint", "Search Plugin..."))
                .OnTextChanged_Lambda([this](const FText& InText) {
                    this->ModList->Filter(InText.ToString());
                })
            ]
            +SHorizontalBox::Slot().AutoWidth()[
                SNew(SCheckBox)
                .Content()[
                    SNew(STextBlock)
                    .Text(LOCTEXT("CheckAllPlugins", "All Content Plugins"))
                ]
                .OnCheckStateChanged_Lambda([this](ECheckBoxState InState) {
                    this->ModList->SetShowEngine(InState == ECheckBoxState::Checked);
                })
            ]
        ]
        +SVerticalBox::Slot().FillHeight(1).Padding(3)[
            SAssignNew(ModList, SGrekitModEntryList)
        ]
    ];
}

#undef LOCTEXT_NAMESPACE
