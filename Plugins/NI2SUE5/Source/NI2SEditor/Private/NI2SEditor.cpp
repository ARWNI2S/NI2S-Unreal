// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SEditor.h"
#include "NI2SEditorStyle.h"
#include "NI2SCommands.h"
#include "EditorModes/NamedEdMode.h"

#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

//TODO: change tab Name
static const FName PluginWinTabName("PluginWin");

#define LOCTEXT_NAMESPACE "FNI2SEditorModule"

void FNI2SEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNI2SEditorStyle::Initialize();
	FNI2SEditorStyle::ReloadTextures();

	FNI2SCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	//MapAction for OpenPluginWindow (Tab)
	PluginCommands->MapAction(
		FNI2SCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNI2SEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	//MapAction for PluginAction (Button)
	PluginCommands->MapAction(
		FNI2SCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FNI2SEditorModule::PluginButtonClicked2),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNI2SEditorModule::RegisterMenus));
	
	//RegisterNomadTab for OpenPluginWindow (Tab)
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PluginWinTabName, FOnSpawnTab::CreateRaw(this, &FNI2SEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FPluginWinTabTitle", "PluginWin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	//Register EditorMode
	//TODO: change texts
	FEditorModeRegistry::Get().RegisterMode<FNamedEdMode>(FNamedEdMode::EM_NamedEdModeId, LOCTEXT("NamedEdModeName", "NamedEdMode"), FSlateIcon(), true);
}

void FNI2SEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FNI2SEditorStyle::Shutdown();

	FNI2SCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PluginWinTabName);

	//Unregister EditorMode
	FEditorModeRegistry::Get().UnregisterMode(FNamedEdMode::EM_NamedEdModeId);
}

TSharedRef<SDockTab> FNI2SEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	//TODO: change tab text
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FNI2SEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("NI2SEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			//TODO: Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FNI2SEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(PluginWinTabName);
}

void FNI2SEditorModule::PluginButtonClicked2()
{
	//TODO: Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
		FText::FromString(TEXT("FNI2SEditorModule::PluginButtonClicked2()")),
		FText::FromString(TEXT("NI2SEditor.cpp"))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FNI2SEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNI2SCommands::Get().OpenPluginWindow, PluginCommands);
			Section.AddMenuEntryWithCommandList(FNI2SCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& EntryWindow = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNI2SCommands::Get().OpenPluginWindow));
				EntryWindow.SetCommandList(PluginCommands);
				FToolMenuEntry& EntryButton = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNI2SCommands::Get().PluginAction));
				EntryButton.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNI2SEditorModule, NI2SEditor)