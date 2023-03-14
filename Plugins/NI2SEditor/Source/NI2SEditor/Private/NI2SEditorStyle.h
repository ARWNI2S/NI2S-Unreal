// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FNI2SEditorStyle
{
public:
	static void Initialize();

	static void Shutdown();

	static TSharedPtr<class ISlateStyle> Get();

	static FName GetStyleSetName();

	static void ReloadTextures();

private:
	static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);

private:
	static TSharedPtr<class FSlateStyleSet> StyleSet;
};