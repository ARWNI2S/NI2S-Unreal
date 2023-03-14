// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "NI2SEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Styling/AppStyle.h"
#include "Interfaces/IPluginManager.h"
#include "SlateOptMacros.h"

#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FNI2SEditorStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )
#define IMAGE_PLUGIN_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush( FNI2SEditorStyle::InContent( RelativePath, ".svg" ), __VA_ARGS__ )
#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

#define RootToContentDir Style->RootToContentDir

FString FNI2SEditorStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin("NI2SEditor")->GetBaseDir() / TEXT("Resources");
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr<FSlateStyleSet> FNI2SEditorStyle::StyleSet = nullptr;
TSharedPtr<class ISlateStyle> FNI2SEditorStyle::Get() { return StyleSet; }

FName FNI2SEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("NI2SEditorStyle"));
	return StyleSetName;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FNI2SEditorStyle::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	//StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin("NI2SEditor")->GetBaseDir() / TEXT("Resources"));
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	const FTextBlockStyle& NormalText = FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

	// Shared editors
	{
		//StyleSet->Set("Paper2D.Common.ViewportZoomTextStyle", FTextBlockStyle(NormalText)
		//	.SetFont(DEFAULT_FONT("BoldCondensed", 16))
		//);

		//StyleSet->Set("Paper2D.Common.ViewportTitleTextStyle", FTextBlockStyle(NormalText)
		//	.SetFont(DEFAULT_FONT("Regular", 18))
		//	.SetColorAndOpacity(FLinearColor(1.0, 1.0f, 1.0f, 0.5f))
		//);

		//StyleSet->Set("Paper2D.Common.ViewportTitleBackground", new BOX_BRUSH("Old/Graph/GraphTitleBackground", FMargin(0)));
	}

	// Test styling
	{
		StyleSet->Set("ClassIcon.TestBlueprint", new IMAGE_PLUGIN_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon16x16));

		StyleSet->Set("NI2SEditor.TestButtonAction", new IMAGE_PLUGIN_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
		StyleSet->Set("NI2SEditor.OpenTestWindow", new IMAGE_PLUGIN_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_PLUGIN_BRUSH
#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef DEFAULT_FONT

void FNI2SEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

void FNI2SEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}
