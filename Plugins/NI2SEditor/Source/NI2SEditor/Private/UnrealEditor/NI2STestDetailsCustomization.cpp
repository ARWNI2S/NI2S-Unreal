// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.


#include "NI2STestDetailsCustomization.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "NI2SEditor"

TSharedRef<IDetailCustomization> FNI2STestDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FNI2STestDetailsCustomization);
}

void FNI2STestDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	TSet<UClass*> Classes;

	TArray<TWeakObjectPtr<UObject>> ObjectsToCustomize;
	DetailLayout.GetObjectsBeingCustomized(/*Out*/ ObjectsToCustomize);

	for (auto WeakObject : ObjectsToCustomize)
	{
		if (UObject* Instance = WeakObject.Get())
		{
			Classes.Add(Instance->GetClass());
		}
	}

	// Create a commands category
	IDetailCategoryBuilder& Category = DetailLayout.EditCategory("Commands");

	for (UClass* Class : Classes)
	{
		for (TFieldIterator<UFunction> FuncIt(Class); FuncIt; ++FuncIt)
		{
			UFunction* Function = *FuncIt;
			if (Function->HasAnyFunctionFlags(FUNC_Exec) && (Function->NumParms == 0))
			{
				const FString FunctionName = Function->GetName();
				const FText ButtonCaption = FText::FromString(FunctionName);
				const FText FilterText = FText::FromString(FunctionName);
				const FString FilterString = FunctionName;

				Category.AddCustomRow(FilterText)
					.ValueContent()
					[
						SNew(SButton)
						.Text(ButtonCaption)
					.OnClicked(FOnClicked::CreateStatic(&FNI2STestDetailsCustomization::ExecuteCommandTool, &DetailLayout, Function))
					];
			}
		}
	}
}

FReply FNI2STestDetailsCustomization::ExecuteCommandTool(IDetailLayoutBuilder* DetailLayout, UFunction* MethodToExecute)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsToCustomize;
	DetailLayout->GetObjectsBeingCustomized(/*Out*/ ObjectsToCustomize);

	for (auto WeakObject : ObjectsToCustomize)
	{
		if (UObject* Instance = WeakObject.Get())
		{
			Instance->CallFunctionByNameWithArguments(*MethodToExecute->GetName(), *GLog, nullptr, true);
		}
	}

	return FReply::Handled();
}
