// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "NI2SEditorModule.h"
#include "NI2SEditorLog.h"
//#include "Engine/Texture2D.h"
//#include "Editor.h"
#include "EditorModeRegistry.h"
//#include "Modules/ModuleManager.h"
//#include "UObject/UObjectHash.h"
//#include "UObject/UObjectIterator.h"
//#include "ThumbnailRendering/ThumbnailManager.h"

#include "AssetToolsModule.h"
//#include "PropertyEditorModule.h"
#include "NI2SEditorStyle.h"
#include "NI2SEditorCommands.h"
//#include "PaperSprite.h"
//#include "PaperEditorShared/SpriteGeometryEditMode.h"
//#include "PaperTileSet.h"

#include "ContentBrowserExtensions/NI2SContentBrowserExtensions.h"
#include "LevelEditorMenuExtensions/NI2SLevelEditorExtensions.h"
//#include "PaperTileMap.h"
//#include "PaperImporterSettings.h"

// NI2SScene support
#include "IAssetTypeActions.h"
//#include "NI2SSceneAssetTypeActions.h"
#include "ComponentAssetBroker.h"
//#include "NI2SSceneComponent.h"
//#include "NI2SSceneAssetBroker.h"
//#include "NI2SSceneThumbnailRenderer.h"
//#include "NI2SSceneEditor/NI2SSceneDetailsCustomization.h"
//#include "NI2SSceneEditor/NI2SSceneComponentDetailsCustomization.h"
//#include "NI2SSceneEditor/NI2SSceneEditorSettings.h"

// NI2SPlot support
//#include "NI2SPlotAssetTypeActions.h"
//#include "NI2SPlotComponent.h"
//#include "NI2SPlot.h"
//#include "NI2SPlotAssetBroker.h"
//#include "NI2SPlotThumbnailRenderer.h"
//#include "NI2SPlotEditor/NI2SPlotComponentDetailsCustomization.h"
//#include "NI2SPlotEditor/NI2SPlotEditorSettings.h"

// Tile set support
//#include "TileSetAssetTypeActions.h"
//#include "PaperTileSetThumbnailRenderer.h"
//#include "TileSetEditor/TileSetEditorSettings.h"
//#include "TileSetEditor/TileSetDetailsCustomization.h"

// Tile map support
//#include "TileMapEditing/TileMapAssetTypeActions.h"
//#include "PaperTileMapComponent.h"
//#include "TileMapEditing/PaperTileMapAssetBroker.h"
//#include "TileMapEditing/EdModeTileMap.h"
//#include "TileMapEditing/PaperTileMapDetailsCustomization.h"
//#include "TileMapEditing/TileMapEditorSettings.h"

// Atlas support
//#include "Atlasing/AtlasAssetTypeActions.h"
//#include "Atlasing/PaperAtlasGenerator.h"
//#include "NI2SSceneAtlas.h"

// Grouped sprite support
//#include "PaperGroupedSpriteComponent.h"
//#include "GroupedSprites/GroupedSpriteDetailsCustomization.h"

// Settings
#include "NI2SEditorSettings.h"
#include "NI2SPluginSettingsDetails.h"
#include "NI2SNetworkSettings.h"
#include "NI2SRuntimeSettings.h"
#include "ISettingsModule.h"

// Mesh paint adapters
//#include "MeshPaintModule.h"
//#include "MeshPainting/MeshPaintSpriteAdapter.h"

//TESTING
#include "Modes/NI2SEditorModeCommands.h"
#include "UnrealEditor/NI2STestDetailsCustomization.h"
#include "ToolMenus.h"

DEFINE_LOG_CATEGORY(LogNI2SEditor);

static const FName TestButtonTabName("NI2SEditorTestButton");
static const FName TestWindowTabName("NI2SEditorTestWindow");
static const FName TestMenuName("Narrative");

#define LOCTEXT_NAMESPACE "NI2SEditor"

//////////////////////////////////////////////////////////////////////////
// FNI2SEditor

class FNI2SEditor : public INI2SEditorModule
{
public:
	FNI2SEditor()
		: NI2SAssetCategoryBit(EAssetTypeCategories::Misc)
	{
	}

	// INI2SEditorModule interface
	virtual TSharedPtr<class FExtensibilityManager> GetNI2SSceneEditorMenuExtensibilityManager() override { return NI2SSceneEditor_MenuExtensibilityManager; }
	virtual TSharedPtr<class FExtensibilityManager> GetNI2SSceneEditorToolBarExtensibilityManager() override { return NI2SSceneEditor_ToolBarExtensibilityManager; }

	virtual TSharedPtr<class FExtensibilityManager> GetNI2SPlotEditorMenuExtensibilityManager() override { return NI2SPlotEditor_MenuExtensibilityManager; }
	virtual TSharedPtr<class FExtensibilityManager> GetNI2SPlotEditorToolBarExtensibilityManager() override { return NI2SPlotEditor_ToolBarExtensibilityManager; }
	virtual uint32 GetNI2SAssetCategory() const override { return NI2SAssetCategoryBit; }
	// End of INI2SEditorModule

private:
	TSharedPtr<FExtensibilityManager> NI2SSceneEditor_MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> NI2SSceneEditor_ToolBarExtensibilityManager;

	TSharedPtr<FExtensibilityManager> NI2SPlotEditor_MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> NI2SPlotEditor_ToolBarExtensibilityManager;

	/** All created asset type actions.  Cached here so that we can unregister them during shutdown. */
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;

	TSharedPtr<IComponentAssetBroker> NI2SSceneBroker;
	TSharedPtr<IComponentAssetBroker> NI2SPlotBroker;
	//TSharedPtr<IComponentAssetBroker> PaperTileMapBroker;

	//TSharedPtr<IMeshPaintGeometryAdapterFactory> NI2SSceneMeshPaintAdapterFactory;
	FDelegateHandle OnPropertyChangedDelegateHandle;
	FDelegateHandle OnAssetReimportDelegateHandle;

	EAssetTypeCategories::Type NI2SAssetCategoryBit;

public:
	/** IModuleInterface implementation */
	//virtual void StartupModule() override
	//{
	//	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FNI2SEditor::OnPostEngineInit);
	//}
	virtual void StartupModule() override
	{
		// Initialize style
		FNI2SEditorStyle::Initialize();
		FNI2SEditorStyle::ReloadTextures();

		// Register Details Customization
		//{
		//	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		//	PropertyModule.RegisterCustomClassLayout("BaseEditorTool", FOnGetDetailCustomizationInstance::CreateStatic(&FNI2STestDetailsCustomization::MakeInstance));
		//	PropertyModule.NotifyCustomizationModuleChanged();
		//}

		// Register Commands
		FNI2SEditorCommands::Register();

		// Begin Action Mappings
		TestCommands = MakeShareable(new FUICommandList);

		// Map TestButtonAction
		TestCommands->MapAction(
			FNI2SEditorCommands::Get().TestButtonAction,
			FExecuteAction::CreateRaw(this, &FNI2SEditor::TestButtonClicked),
			FCanExecuteAction());

		// Map OpenTestWindow
		TestCommands->MapAction(
			FNI2SEditorCommands::Get().OpenTestWindow,
			FExecuteAction::CreateRaw(this, &FNI2SEditor::OpenTestWindowClicked),
			FCanExecuteAction());

		// Register EdMode Commands
		FNI2SEditorModeCommands::Register();

		// Register Tool Menus Startup Callback
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNI2SEditor::RegisterMenus));

		// Register Tab Spawn
		FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestWindowTabName, FOnSpawnTab::CreateRaw(this, &FNI2SEditor::OnSpawnTestTab))
			.SetDisplayName(LOCTEXT("TestWindowTabTitle", "NI2SEditor"))
			.SetMenuType(ETabSpawnerMenuType::Hidden);

		RegisterSettings();

		// Settings Layout ;-P
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(UNI2SPluginSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FNI2SPluginSettingsDetails::MakeInstance));
	}

	void OnPostEngineInit()
	{
		NI2SSceneEditor_MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		NI2SSceneEditor_ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

		NI2SPlotEditor_MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		NI2SPlotEditor_ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

		// Register slate style overrides
		FNI2SEditorStyle::Initialize();
		//FNI2SEditorStyle::ReloadTextures(); TODO: Really needed?

		// Register commands
		FNI2SEditorCommands::Register();

		// Register asset types
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		NI2SAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("NI2S")), LOCTEXT("NI2SAssetCategory", "NI2S"));

		//RegisterAssetTypeAction(AssetTools, MakeShareable(new FNI2SSceneAssetTypeActions(NI2SAssetCategoryBit)));
		//RegisterAssetTypeAction(AssetTools, MakeShareable(new FNI2SPlotAssetTypeActions(NI2SAssetCategoryBit)));
		//RegisterAssetTypeAction(AssetTools, MakeShareable(new FTileSetAssetTypeActions(NI2SAssetCategoryBit)));
		//RegisterAssetTypeAction(AssetTools, MakeShareable(new FTileMapAssetTypeActions(NI2SAssetCategoryBit)));
		//RegisterAssetTypeAction(AssetTools, MakeShareable(new FAtlasAssetTypeActions(NI2SAssetCategoryBit)));

		//NI2SSceneBroker = MakeShareable(new FNI2SSceneAssetBroker);
		//FComponentAssetBrokerage::RegisterBroker(NI2SSceneBroker, UNI2SSceneComponent::StaticClass(), true, true);

		//NI2SPlotBroker = MakeShareable(new FNI2SPlotAssetBroker);
		//FComponentAssetBrokerage::RegisterBroker(NI2SPlotBroker, UNI2SPlotComponent::StaticClass(), true, true);

		//PaperTileMapBroker = MakeShareable(new FPaperTileMapAssetBroker);
		//FComponentAssetBrokerage::RegisterBroker(PaperTileMapBroker, UPaperTileMapComponent::StaticClass(), true, true);

		// Register the details customizations
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			//PropertyModule.RegisterCustomClassLayout(UPaperTileMapComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPaperTileMapDetailsCustomization::MakeInstance));
			//PropertyModule.RegisterCustomClassLayout(UPaperTileMap::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FPaperTileMapDetailsCustomization::MakeInstance));
			//PropertyModule.RegisterCustomClassLayout(UPaperTileSet::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTileSetDetailsCustomization::MakeInstance));
			//PropertyModule.RegisterCustomClassLayout(UNI2SScene::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FSpriteDetailsCustomization::MakeInstance));
			//PropertyModule.RegisterCustomClassLayout(UNI2SSceneComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FSpriteComponentDetailsCustomization::MakeInstance));
			//PropertyModule.RegisterCustomClassLayout(UNI2SPlotComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FFlipbookComponentDetailsCustomization::MakeInstance));
			//PropertyModule.RegisterCustomClassLayout(UPaperGroupedSpriteComponent::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGroupedSpriteComponentDetailsCustomization::MakeInstance));

			//@TODO: Struct registration should happen using ::StaticStruct, not by string!!!
			//PropertyModule.RegisterCustomPropertyTypeLayout( "SpritePolygonCollection", FOnGetPropertyTypeCustomizationInstance::CreateStatic( &FSpritePolygonCollectionCustomization::MakeInstance ) );

			PropertyModule.NotifyCustomizationModuleChanged();
		}

		// Register to be notified when properties are edited
		//OnPropertyChangedDelegateHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &FPaper2DEditor::OnPropertyChanged);

		// Register to be notified when an asset is reimported
		if (GEditor)
		{
			//OnAssetReimportDelegateHandle = GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetReimport.AddRaw(this, &FPaper2DEditor::OnObjectReimported);
		}

		// Register the thumbnail renderers
		//UThumbnailManager::Get().RegisterCustomRenderer(UNI2SScene::StaticClass(), UNI2SSceneThumbnailRenderer::StaticClass());
		//UThumbnailManager::Get().RegisterCustomRenderer(UPaperTileSet::StaticClass(), UPaperTileSetThumbnailRenderer::StaticClass());
		//UThumbnailManager::Get().RegisterCustomRenderer(UNI2SPlot::StaticClass(), UNI2SPlotThumbnailRenderer::StaticClass());
		//@TODO: PAPER2D: UThumbnailManager::Get().RegisterCustomRenderer(UPaperTileMap::StaticClass(), UPaperTileMapThumbnailRenderer::StaticClass());

		// Register the editor modes
		//FEditorModeRegistry::Get().RegisterMode<FEdModeTileMap>(
		//	FEdModeTileMap::EM_TileMap,
		//	LOCTEXT("TileMapEditMode", "Tile Map Editor"),
		//	FSlateIcon(),
		//	false);

		//FEditorModeRegistry::Get().RegisterMode<FSpriteGeometryEditMode>(
		//	FSpriteGeometryEditMode::EM_SpriteGeometry,
		//	LOCTEXT("SpriteGeometryEditMode", "Sprite Geometry Editor"),
		//	FSlateIcon(),
		//	false);

		// Integrate Paper2D actions into existing editor context menus
		if (!IsRunningCommandlet())
		{
			//FPaperContentBrowserExtensions::InstallHooks();
			//FPaperLevelEditorMenuExtensions::InstallHooks();
		}
		// Register with the mesh paint module
		//if (IMeshPaintModule* MeshPaintModule = FModuleManager::LoadModulePtr<IMeshPaintModule>("MeshPaint"))
		//{
		//	SpriteMeshPaintAdapterFactory = MakeShareable(new FMeshPaintSpriteAdapterFactory());
		//	MeshPaintModule->RegisterGeometryAdapterFactory(SpriteMeshPaintAdapterFactory.ToSharedRef());
		//}

		RegisterSettings();
	}

	virtual void ShutdownModule() override
	{
		// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
		// we call this function before unloading the module.


		if (UObjectInitialized())
		{
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
			
			UnregisterSettings();

			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomClassLayout(UNI2SPluginSettings::StaticClass()->GetFName());

		}

	}

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}

	// Called when a property on the specified object is modified
	void OnPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
	{
		//if (UPaperSpriteAtlas* Atlas = Cast<UPaperSpriteAtlas>(ObjectBeingModified))
		//{
		//	FPaperAtlasGenerator::HandleAssetChangedEvent(Atlas);
		//}
		//else if (UPaperRuntimeSettings* Settings = Cast<UPaperRuntimeSettings>(ObjectBeingModified))
		//{
		//	// Handle changes to experimental flags here
		//}
	}

	void OnObjectReimported(UObject* InObject)
	{
		//if (UTexture2D* Texture = Cast<UTexture2D>(InObject))
		//{
		//	for (TObjectIterator<UPaperSprite> SpriteIt; SpriteIt; ++SpriteIt)
		//	{
		//		SpriteIt->OnObjectReimported(Texture);
		//	}
		//}
	}

	void RegisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			// Plugin Settings
			SettingsModule->RegisterSettings("Project", "Plugins", "NI2S",
				LOCTEXT("NI2SPluginSettingsName", "NI2S Plugin"),
				LOCTEXT("NI2SPluginSettingsDescription", "NI2S plugin settings."),
				GetMutableDefault<UNI2SPluginSettings>());

			// Editor Plugin Settings
			SettingsModule->RegisterSettings("Editor", "Plugins", "NI2S",
				LOCTEXT("NI2SEditorSettingsName", "NI2S Editor Settings"),
				LOCTEXT("NI2SEditorSettingsDescription", "Configure Unreal Editor for NI2S plugin."),
				GetMutableDefault<UNI2SEditorSettings>());

			// Project Runtime Settings
			SettingsModule->RegisterSettings("Project", "NI2S", "Runtime",
				LOCTEXT("NI2SRuntimeSettingsName", "Runtime Settings"),
				LOCTEXT("NI2SRuntimeSettingsDescription", "Configure the NI2S Runtime"),
				GetMutableDefault<UNI2SRuntimeSettings>());

			// Project Network Settings
			SettingsModule->RegisterSettings("Project", "NI2S", "Network",
				LOCTEXT("NI2SNetworkSettingsName", "Network Settings"),
				LOCTEXT("NI2SNetworkSettingsDescription", "Configure the NI2S Network"),
				GetMutableDefault<UNI2SNetworkSettings>());

			// Editor Narrative Tools Settings
			//SettingsModule->RegisterSettings("Editor", "ContentEditors", "SpriteEditor",
			//	LOCTEXT("SpriteEditorSettingsName", "Sprite Editor"),
			//	LOCTEXT("SpriteEditorSettingsDescription", "Configure the look and feel of the Sprite Editor."),
			//	GetMutableDefault<USpriteEditorSettings>());

			// Editor Narrative Tools Settings
			//SettingsModule->RegisterSettings("Editor", "ContentEditors", "FlipbookEditor",
			//	LOCTEXT("FlipbookEditorSettingsName", "Flipbook Editor"),
			//	LOCTEXT("FlipbookEditorSettingsDescription", "Configure the look and feel of the Flipbook Editor."),
			//	GetMutableDefault<UFlipbookEditorSettings>());

			// Editor Narrative Tools Settings
			//SettingsModule->RegisterSettings("Project", "Editor", "Paper2DImport",
			//	LOCTEXT("PaperImporterSettingsName", "Paper2D - Import"),
			//	LOCTEXT("PaperImporterSettingsDescription", "Configure how assets get imported or converted to sprites."),
			//	GetMutableDefault<UPaperImporterSettings>());
		}
	}

	void UnregisterSettings()
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			//SettingsModule->UnregisterSettings("Project", "Editor", "Paper2DImport");
			//SettingsModule->UnregisterSettings("Editor", "ContentEditors", "FlipbookEditor");
			//SettingsModule->UnregisterSettings("Editor", "ContentEditors", "SpriteEditor");
			SettingsModule->UnregisterSettings("Editor", "NI2S", "Global");
			SettingsModule->UnregisterSettings("Project", "NI2S", "Network");
			SettingsModule->UnregisterSettings("Project", "NI2S", "Runtime");
			SettingsModule->UnregisterSettings("Project", "Plugins", "NI2S");
		}
	}

public:
	/** INI2SEditorModule implementation */
	void TestButtonClicked() override
	{
		// Put your "OnButtonClicked" stuff here
		FText DialogText = FText::Format(
			LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
			FText::FromString(TEXT("FNI2SEditor::TestButtonClicked()")),
			FText::FromString(TEXT("NI2SEditorModule.cpp"))
		);
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	}
	void OpenTestWindowClicked() override
	{
		FGlobalTabmanager::Get()->TryInvokeTab(TestWindowTabName);
	}

private:

	void RegisterMenus()
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

	TSharedRef<SDockTab> OnSpawnTestTab(const FSpawnTabArgs& SpawnTabArgs)
	{
		FText WidgetText = FText::Format(
			LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
			FText::FromString(TEXT("FNI2SEditor::OnSpawnTestTab")),
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

private:
	TSharedPtr<class FUICommandList> TestCommands;


};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FNI2SEditor, NI2SEditor)

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
