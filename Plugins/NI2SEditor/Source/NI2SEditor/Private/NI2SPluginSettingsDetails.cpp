// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "NI2SPluginSettingsDetails.h"
#include "NI2SEditorHelpers.h"
#include "NI2SEditorSettings.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "NI2SPluginSettingsDetails"

TSharedRef<IDetailCustomization> FNI2SPluginSettingsDetails::MakeInstance()
{
	return MakeShareable(new FNI2SPluginSettingsDetails);
}

void FNI2SPluginSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);
	check(ObjectsBeingCustomized.Num() == 1);
	TWeakObjectPtr<UNI2SPluginSettings> Settings = Cast<UNI2SPluginSettings>(ObjectsBeingCustomized[0].Get());

	IDetailCategoryBuilder& RuntimeCategory = DetailBuilder.EditCategory("Runtime");

	//TSharedPtr<IPropertyHandle> IsInitHandle = RuntimeCategory.GetProperty("bIsNI2SInitialized");

	RuntimeCategory.AddCustomRow(LOCTEXT("NI2SRuntimeInitializer", "NI2SRuntimeInitializer"))
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(5)
			.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("InitializeNI2S", "Initialize NI2S"))
				.ToolTipText(LOCTEXT("InitializeNI2S_Tooltip", "(Re)Initialize NI2S Runtime"))
				.IsFocusable(false)
				.IsEnabled_Lambda([this, Settings]()
				{
					return !Settings->bIsNI2SInitialized;
				})
				.OnClicked_Lambda([this, Settings]()
				{
					if (Settings.IsValid() && NI2SEditorHelpers::InitializeNI2SPlugin(Settings->bIsNI2SInitialized))
					{
						Settings->TryUpdateDefaultConfigFile();
					}
					else
					{
						// TODO: toast?
					}

					return(FReply::Handled());
				})
			]
			+ SHorizontalBox::Slot()
			.Padding(5)
			.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("ResetNI2S", "Reset NI2S"))
				.ToolTipText(LOCTEXT("ResetNI2S_Tooltip", "Reset the current NI2S editor settings. May require initialization."))
				.IsFocusable(false)
				.IsEnabled_Lambda([this, Settings]()
				{
					return Settings->bIsNI2SInitialized;
				})
				.OnClicked_Lambda([this, Settings]()
				{
					if (Settings.IsValid() && NI2SEditorHelpers::ResetNI2SPlugin(Settings->bIsNI2SInitialized))
					{
						Settings->SaveConfig(CPF_Config, *Settings->GetDefaultConfigFilename());
					}
					else
					{
						// TODO: toast?
					}

					return(FReply::Handled());
				})
			]
		];
}

#undef LOCTEXT_NAMESPACE
