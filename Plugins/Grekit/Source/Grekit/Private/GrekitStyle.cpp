#include "GrekitStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

static FName StyleSetName(TEXT("GrekitStyle"));
TSharedPtr< FSlateStyleSet > FGrekitStyle::StyleInstance = NULL;

void FGrekitStyle::Initialize() {
    if (!StyleInstance.IsValid()) {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FGrekitStyle::Shutdown() {
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

FName FGrekitStyle::GetStyleSetName() {
    return StyleSetName;
}

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__)

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon128x128(40.0f, 40.0f);

bool ShouldUseHatIcon() {
    const FDateTime DateTime = FDateTime::Now();
    return (DateTime.GetMonthOfYear() == EMonthOfYear::December && DateTime.GetDay() >= 15) ||
        (DateTime.GetMonthOfYear() == EMonthOfYear::January && DateTime.GetDay() <= 15);
}

TSharedRef<FSlateStyleSet> FGrekitStyle::Create() {
    TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(StyleSetName));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("Grekit"))->GetBaseDir() / TEXT("Resources"));
    const FString IconName = ShouldUseHatIcon() ? TEXT("GrekitHat40") : TEXT("Grekit40");
    Style->Set(TEXT("Grekit.OpenPluginWindow"), new IMAGE_BRUSH(IconName, Icon40x40));
    return Style;
}

void FGrekitStyle::ReloadTextures() {
    if (FSlateApplication::IsInitialized()) {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

const ISlateStyle& FGrekitStyle::Get() {
    return *StyleInstance;
}
