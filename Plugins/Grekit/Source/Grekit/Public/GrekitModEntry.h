#pragma once
#include "GrekitModEntryList.h"
#include "Interfaces/IPluginManager.h"

/**
 * Entry of a mod within the mod list of the grekit window
 */
class SGrekitModEntry : public SCompoundWidget {
    SLATE_BEGIN_ARGS(SGrekitModEntry) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& Args, TSharedRef<IPlugin> InMod, TSharedRef<SGrekitModEntryList> InOwner);

    void PackageMod(const TArray<TSharedPtr<SGrekitModEntry>>& NextEntries) const;
    void OnEnableCheckboxChanged(ECheckBoxState NewState);

    FORCEINLINE bool IsSelected() {
        return Checkbox && Checkbox->IsChecked();
    }

    FORCEINLINE void SetSelected(bool bInSelected) {
        if (Checkbox) {
            Checkbox->SetIsChecked(bInSelected);
        }
    }

private:
    TSharedPtr<IPlugin> Mod;
    TSharedPtr<SGrekitModEntryList> Owner;
    TSharedPtr<class SCheckBox> Checkbox;
};
