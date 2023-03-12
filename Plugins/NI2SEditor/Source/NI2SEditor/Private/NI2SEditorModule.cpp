// Copyright Epic Games, Inc. All Rights Reserved.

#include "NI2SEditorModule.h"
#include "NI2SEditorStyle.h"
#include "NI2SEditorCommands.h"
#include "Modes/NI2SEditorModeCommands.h"
#include "ToolMenus.h"

static const FName TestButtonTabName("NI2SEditorTestButton");
static const FName TestWindowTabName("NI2SEditorTestWindow");

#define LOCTEXT_NAMESPACE "NI2SEditorModule"

void FNI2SEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Initialize style
	FNI2SEditorStyle::Initialize();
	FNI2SEditorStyle::ReloadTextures();

	// Register Commands
	FNI2SEditorCommands::Register();

	// Begin Action Mappings
	TestCommands = MakeShareable(new FUICommandList);

	// Map TestButtonAction
	TestCommands->MapAction(
		FNI2SEditorCommands::Get().TestButtonAction,
		FExecuteAction::CreateRaw(this, &FNI2SEditorModule::TestButtonClicked),
		FCanExecuteAction());

	// Map OpenTestWindow
	TestCommands->MapAction(
		FNI2SEditorCommands::Get().OpenTestWindow,
		FExecuteAction::CreateRaw(this, &FNI2SEditorModule::OpenTestWindowClicked),
		FCanExecuteAction());

	// Register EdMode Commands
	FNI2SEditorModeCommands::Register();

	// Register Tool Menus Startup Callback
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNI2SEditorModule::RegisterMenus));

	// Register Tab Spawn
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestWindowTabName, FOnSpawnTab::CreateRaw(this, &FNI2SEditorModule::OnSpawnTestTab))
		.SetDisplayName(LOCTEXT("TestWindowTabTitle", "NI2SEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FNI2SEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// UnRegister Nomad Tabs
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestWindowTabName);

	// UnRegister Tool Menus
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	// UnRegister EdMode Commands
	FNI2SEditorModeCommands::Unregister();

	// UnRegister Commands
	FNI2SEditorCommands::Unregister();

	// Shutdown style
	FNI2SEditorStyle::Shutdown();
}

TSharedRef<SDockTab> FNI2SEditorModule::OnSpawnTestTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FNI2SEditorModule::OnSpawnTestTab")),
		FText::FromString(TEXT("NI2SEditorModule.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(WidgetText)
		]
		];
}

void FNI2SEditorModule::TestButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
		FText::FromString(TEXT("FNI2SEditorModule::TestButtonClicked()")),
		FText::FromString(TEXT("NI2SEditorModule.cpp"))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FNI2SEditorModule::OpenTestWindowClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(TestWindowTabName);
}

void FNI2SEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNI2SEditorCommands::Get().TestButtonAction, TestCommands);
			Section.AddMenuEntryWithCommandList(FNI2SEditorCommands::Get().OpenTestWindow, TestCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section1 = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section1.AddEntry(FToolMenuEntry::InitToolBarButton(FNI2SEditorCommands::Get().TestButtonAction));
				Entry.SetCommandList(TestCommands);
			}
			FToolMenuSection& Section2 = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section2.AddEntry(FToolMenuEntry::InitToolBarButton(FNI2SEditorCommands::Get().OpenTestWindow));
				Entry.SetCommandList(TestCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNI2SEditorModule, NI2SEditorMode)