#pragma once

#include "GrekitModEntryList.h"
#include "Slate.h"

class SGrekitWidget : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(SGrekitWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TSharedPtr<IDetailsView> DetailsView;
    TSharedPtr<SGrekitModEntryList> ModList;
};
