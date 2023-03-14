// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "NI2SContentBrowserExtensions.h"
#include "NI2SEditorStyle.h"
#include "Modules/ModuleManager.h"
#include "Misc/PackageName.h"
#include "Textures/SlateIcon.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Styling/AppStyle.h"
#include "Engine/Texture2D.h"
#include "AssetRegistry/AssetData.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "NI2SEditor"

DECLARE_LOG_CATEGORY_EXTERN(LogNI2SCBExtensions, Log, All);
DEFINE_LOG_CATEGORY(LogNI2SCBExtensions);

//////////////////////////////////////////////////////////////////////////

static FContentBrowserMenuExtender_SelectedAssets ContentBrowserExtenderDelegate;
static FDelegateHandle ContentBrowserExtenderDelegateHandle;

//////////////////////////////////////////////////////////////////////////
// FContentBrowserSelectedAssetExtensionBase

struct FContentBrowserSelectedAssetExtensionBase
{
public:
	TArray<struct FAssetData> SelectedAssets;

public:
	virtual void Execute() {}
	virtual ~FContentBrowserSelectedAssetExtensionBase() {}
};

//////////////////////////////////////////////////////////////////////////
// FCreateSpriteFromTextureExtension

#include "IAssetTools.h"
#include "AssetToolsModule.h"


//struct FCreateSpriteFromTextureExtension : public FContentBrowserSelectedAssetExtensionBase
//{
//	//bool bExtractSprites;
//
//	FCreateSpriteFromTextureExtension()
//		//: bExtractSprites(false)
//	{
//	}
//
//	//void CreateSpritesFromTextures(TArray<UTexture2D*>& Textures)
//	//{
//	//	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
//	//	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
//
//	//	TArray<UObject*> ObjectsToSync;
//
//	//	for (auto TextureIt = Textures.CreateConstIterator(); TextureIt; ++TextureIt)
//	//	{
//	//		UTexture2D* Texture = *TextureIt;
//
//	//		// Create the factory used to generate the sprite
//	//		UPaperSpriteFactory* SpriteFactory = NewObject<UPaperSpriteFactory>();
//	//		SpriteFactory->InitialTexture = Texture;
//
//	//		// Create the sprite
//	//		FString Name;
//	//		FString PackageName;
//
//	//		if (!bExtractSprites)
//	//		{
//	//			// Get a unique name for the sprite
//	//			const FString DefaultSuffix = TEXT("_Sprite");
//	//			AssetToolsModule.Get().CreateUniqueAssetName(Texture->GetOutermost()->GetName(), DefaultSuffix, /*out*/ PackageName, /*out*/ Name);
//	//			const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
//
//	//			if (UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PackagePath, UPaperSprite::StaticClass(), SpriteFactory))
//	//			{
//	//				ObjectsToSync.Add(NewAsset);
//	//			}
//	//		}
//	//		else
//	//		{
//	//			SPaperExtractSpritesDialog::ShowWindow(Texture);
//	//		}
//	//	}
//
//	//	if (ObjectsToSync.Num() > 0)
//	//	{
//	//		ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
//	//	}
//	//}
//
//	virtual void Execute() override
//	{
//		//// Create sprites for any selected textures
//		//TArray<UTexture2D*> Textures;
//		//for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
//		//{
//		//	const FAssetData& AssetData = *AssetIt;
//		//	if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
//		//	{
//		//		Textures.Add(Texture);
//		//	}
//		//}
//
//		//CreateSpritesFromTextures(Textures);
//	}
//};
//
//struct FConfigureTexturesForSpriteUsageExtension : public FContentBrowserSelectedAssetExtensionBase
//{
//	virtual void Execute() override
//	{
//		//// Change the compression settings and trigger a recompress
//		//for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
//		//{
//		//	const FAssetData& AssetData = *AssetIt;
//		//	if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
//		//	{
//		//		GetDefault<UPaperImporterSettings>()->ApplyTextureSettings(Texture);
//		//	}
//		//}
//	}
//};
//
//struct FCreateTileSetFromTextureExtension : public FContentBrowserSelectedAssetExtensionBase
//{
//	//void CreateTileSetsFromTextures(TArray<UTexture2D*>& Textures)
//	//{
//	//	const FString DefaultSuffix = TEXT("_TileSet");
//
//	//	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
//	//	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
//
//	//	TArray<UObject*> ObjectsToSync;
//
//	//	for (auto TextureIt = Textures.CreateConstIterator(); TextureIt; ++TextureIt)
//	//	{
//	//		UTexture2D* Texture = *TextureIt;
//
//	//		// Create the factory used to generate the tile set
//	//		UPaperTileSetFactory* TileSetFactory = NewObject<UPaperTileSetFactory>();
//	//		TileSetFactory->InitialTexture = Texture;
//
//	//		// Get a unique name for the tile set
//	//		FString Name;
//	//		FString PackageName;
//	//		AssetToolsModule.Get().CreateUniqueAssetName(Texture->GetOutermost()->GetName(), DefaultSuffix, /*out*/ PackageName, /*out*/ Name);
//	//		const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
//
//	//		if (UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PackagePath, UPaperTileSet::StaticClass(), TileSetFactory))
//	//		{
//	//			ObjectsToSync.Add(NewAsset);
//	//		}
//	//	}
//
//	//	if (ObjectsToSync.Num() > 0)
//	//	{
//	//		ContentBrowserModule.Get().SyncBrowserToAssets(ObjectsToSync);
//	//	}
//	//}
//
//	virtual void Execute() override
//	{
//		//// Create sprites for any selected textures
//		//TArray<UTexture2D*> Textures;
//		//for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
//		//{
//		//	const FAssetData& AssetData = *AssetIt;
//		//	if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
//		//	{
//		//		Textures.Add(Texture);
//		//	}
//		//}
//
//		//CreateTileSetsFromTextures(Textures);
//	}
//};

//////////////////////////////////////////////////////////////////////////
// FNI2SContentBrowserExtensions_Impl

class FNI2SContentBrowserExtensions_Impl
{
public:
	static void ExecuteSelectedContentFunctor(TSharedPtr<FContentBrowserSelectedAssetExtensionBase> SelectedAssetFunctor)
	{
		SelectedAssetFunctor->Execute();
	}

	//static void CreateSpriteActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	//{
	//	MenuBuilder.AddSubMenu(
	//		LOCTEXT("SpriteActionsSubMenuLabel", "Sprite Actions"),
	//		LOCTEXT("SpriteActionsSubMenuToolTip", "Sprite-related actions for this texture."),
	//		FNewMenuDelegate::CreateStatic(&FNI2SContentBrowserExtensions_Impl::PopulateNI2SActionsMenu, SelectedAssets),
	//		false,
	//		FSlateIcon(FNI2SEditorStyle::Get()->GetStyleSetName(), "ClassIcon.PaperSprite")
	//	);
	//}
	static void CreateNI2SActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		MenuBuilder.AddSubMenu(
			LOCTEXT("NI2SActionsSubMenuLabel", "NI2S Actions"),
			LOCTEXT("NI2SActionsSubMenuToolTip", "NI2S-related actions for this blueprint."),
			FNewMenuDelegate::CreateStatic(&FNI2SContentBrowserExtensions_Impl::PopulateNI2SActionsMenu, SelectedAssets),
			false,
			FSlateIcon(FNI2SEditorStyle::Get()->GetStyleSetName(), "ClassIcon.TestBlueprint")
		);
	}
	
	static void PopulateNI2SActionsMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		//// Create sprites
		//TSharedPtr<FCreateSpriteFromTextureExtension> SpriteCreatorFunctor = MakeShareable(new FCreateSpriteFromTextureExtension());
		//SpriteCreatorFunctor->SelectedAssets = SelectedAssets;

		//FUIAction Action_CreateSpritesFromTextures(
		//	FExecuteAction::CreateStatic(&FNI2SContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(SpriteCreatorFunctor)));

		//const FName NI2SEditorStyleSetName = FNI2SEditorStyle::Get()->GetStyleSetName();

		//MenuBuilder.AddMenuEntry(
		//	LOCTEXT("CB_Extension_Texture_CreateSprite", "Create Sprite"),
		//	LOCTEXT("CB_Extension_Texture_CreateSprite_Tooltip", "Create sprites from selected textures"),
		//	FSlateIcon(NI2SEditorStyleSetName, "AssetActions.CreateSprite"),
		//	Action_CreateSpritesFromTextures,
		//	NAME_None,
		//	EUserInterfaceActionType::Button);

		//// Extract Sprites
		//TSharedPtr<FCreateSpriteFromTextureExtension> SpriteExtractorFunctor = MakeShareable(new FCreateSpriteFromTextureExtension());
		//SpriteExtractorFunctor->SelectedAssets = SelectedAssets;
		//SpriteExtractorFunctor->bExtractSprites = true;

		//FUIAction Action_ExtractSpritesFromTextures(
		//	FExecuteAction::CreateStatic(&FNI2SContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(SpriteExtractorFunctor)));

		//MenuBuilder.AddMenuEntry(
		//	LOCTEXT("CB_Extension_Texture_ExtractSprites", "Extract Sprites"),
		//	LOCTEXT("CB_Extension_Texture_ExtractSprites_Tooltip", "Extract sprites from selected textures"),
		//	FSlateIcon(NI2SEditorStyleSetName, "AssetActions.ExtractSprites"),
		//	Action_ExtractSpritesFromTextures,
		//	NAME_None,
		//	EUserInterfaceActionType::Button);

		//// Configure the selected textures according to the project settings (same as if it got imported from a sprite sheet)
		//TSharedPtr<FConfigureTexturesForSpriteUsageExtension> TextureConfigFunctor = MakeShareable(new FConfigureTexturesForSpriteUsageExtension());
		//TextureConfigFunctor->SelectedAssets = SelectedAssets;

		//FUIAction Action_ConfigureTexturesForSprites(
		//	FExecuteAction::CreateStatic(&FNI2SContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(TextureConfigFunctor)));

		//MenuBuilder.AddMenuEntry(
		//	LOCTEXT("CB_Extension_Texture_ConfigureTextureForSprites", "Apply Paper2D Texture Settings"),
		//	LOCTEXT("CB_Extension_Texture_ConfigureTextureForSprites_Tooltip", "Sets compression settings and sampling modes to the defaults specified in the 'Paper2D - Import' project settings"),
		//	FSlateIcon(NI2SEditorStyleSetName, "AssetActions.ConfigureForRetroSprites"),
		//	Action_ConfigureTexturesForSprites,
		//	NAME_None,
		//	EUserInterfaceActionType::Button);

		//// Create sprites
		//TSharedPtr<FCreateTileSetFromTextureExtension> TileSetCreatorFunctor = MakeShareable(new FCreateTileSetFromTextureExtension());
		//TileSetCreatorFunctor->SelectedAssets = SelectedAssets;

		//FUIAction Action_CreateTileSetFromTextures(
		//	FExecuteAction::CreateStatic(&FNI2SContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(TileSetCreatorFunctor)));

		//MenuBuilder.AddMenuEntry(
		//	LOCTEXT("CB_Extension_Texture_CreateTileSet", "Create Tile Set"),
		//	LOCTEXT("CB_Extension_Texture_CreateTileSet_Tooltip", "Create tile set from selected texture"),
		//	FSlateIcon(NI2SEditorStyleSetName, "AssetActions.CreateTileSet"),
		//	Action_CreateTileSetFromTextures,
		//	NAME_None,
		//	EUserInterfaceActionType::Button);
	}

	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender(new FExtender());

		// Run thru the assets to determine if any meet our criteria
		bool bAnyBlueprints = false;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& Asset = *AssetIt;
			bAnyBlueprints = bAnyBlueprints || (Asset.AssetClassPath == UBlueprint::StaticClass()->GetClassPathName());
		}

		bool bAnyNarrative = false;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& Asset = *AssetIt;
			//bAnyBlueprints = bAnyBlueprints || (Asset.AssetClassPath == UBlueprint::StaticClass()->GetClassPathName());
		}

		if (bAnyNarrative)
		{
			// Add the sprite actions sub-menu extender
			bAnyBlueprints = false;
		}
		if (bAnyBlueprints)
		{
			// Add the sprite actions sub-menu extender
			Extender->AddMenuExtension(
				"GetAssetActions",
				EExtensionHook::After,
				nullptr,
				FMenuExtensionDelegate::CreateStatic(&FNI2SContentBrowserExtensions_Impl::CreateNI2SActionsSubMenu, SelectedAssets));
		}

		//// Run thru the assets to determine if any meet our criteria
		//bool bAnyTextures = false;
		//for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		//{
		//	const FAssetData& Asset = *AssetIt;
		//	bAnyTextures = bAnyTextures || (Asset.AssetClassPath == UTexture2D::StaticClass()->GetClassPathName());
		//}

		//if (bAnyTextures)
		//{
		//	// Add the sprite actions sub-menu extender
		//	Extender->AddMenuExtension(
		//		"GetAssetActions",
		//		EExtensionHook::After,
		//		nullptr,
		//		FMenuExtensionDelegate::CreateStatic(&FNI2SContentBrowserExtensions_Impl::CreateSpriteActionsSubMenu, SelectedAssets));
		//}

		return Extender;
	}

	static TArray<FContentBrowserMenuExtender_SelectedAssets>& GetExtenderDelegates()
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		return ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	}
};

//////////////////////////////////////////////////////////////////////////
// FNI2SContentBrowserExtensions

void FNI2SContentBrowserExtensions::InstallHooks()
{
	ContentBrowserExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FNI2SContentBrowserExtensions_Impl::OnExtendContentBrowserAssetSelectionMenu);

	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FNI2SContentBrowserExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.Add(ContentBrowserExtenderDelegate);
	ContentBrowserExtenderDelegateHandle = CBMenuExtenderDelegates.Last().GetHandle();
}

void FNI2SContentBrowserExtensions::RemoveHooks()
{
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FNI2SContentBrowserExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
}

#undef LOCTEXT_NAMESPACE
