// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#include "NI2SEditorSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NI2SEditorSettings)

//////////////////////////////////////////////////////////////////////////
// UNI2SEditorSettings

UNI2SEditorSettings::UNI2SEditorSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bEnableFullAutomation(false)
	, bTestOption(false)
{
}

//////////////////////////////////////////////////////////////////////////
// UNI2SPluginSettings

UNI2SPluginSettings::UNI2SPluginSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsNI2SInitialized(false)
	, bTestOption(false)
{
}

void UNI2SPluginSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetName() == TEXT("SecondaryEncryptionKeys"))
	//{
	//	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
	//	{
	//		int32 Index = PropertyChangedEvent.GetArrayIndex(TEXT("SecondaryEncryptionKeys"));
	//		CryptoKeysHelpers::GenerateEncryptionKey(SecondaryEncryptionKeys[Index].Key);

	//		int32 Number = 1;
	//		FString NewName = FString::Printf(TEXT("New Encryption Key %d"), Number++);
	//		while (SecondaryEncryptionKeys.FindByPredicate([NewName](const FCryptoEncryptionKey& Key) { return Key.Name == NewName; }) != nullptr)
	//		{
	//			NewName = FString::Printf(TEXT("New Encryption Key %d"), Number++);
	//		}

	//		SecondaryEncryptionKeys[Index].Name = NewName;
	//		SecondaryEncryptionKeys[Index].Guid = FGuid::NewGuid();
	//	}
	//}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
