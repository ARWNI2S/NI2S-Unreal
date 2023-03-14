// Copyrigth (c) 2023 Alternate Reality Worlds. Narrative Interactive Intelligent Simulator for Unreal Engine Plugin.

#pragma once

#include "Engine.h"
#include "IDetailCustomization.h"

//////////////////////////////////////////////////////////////////////////
// FNI2STestDetailsCustomization

class FNI2STestDetailsCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	static FReply ExecuteCommandTool(IDetailLayoutBuilder* DetailLayout, UFunction* MethodToExecute);

	// IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End of IDetailCustomization interface
};
