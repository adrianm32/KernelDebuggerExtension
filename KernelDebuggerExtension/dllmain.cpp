#include "stdafx.h"
#include "dllmain.h"

// these are defined globally here. In header file they are declared as extern.
PDEBUG_CONTROL DebugControl;
PDEBUG_SYMBOLS DebugSymbols;
PDEBUG_DATA_SPACES DebugDataSpaces;
PDEBUG_REGISTERS DebugRegisters;

_Check_return_ HRESULT QueryInterfaces(_In_ PDEBUG_CLIENT DebugClient)
{
	HRESULT hr = S_OK;

	IFC(DebugClient->QueryInterface(IID_PPV_ARGS(&DebugControl)));
	IFC(DebugClient->QueryInterface(IID_PPV_ARGS(&DebugSymbols)));
	IFC(DebugClient->QueryInterface(IID_PPV_ARGS(&DebugDataSpaces)));
	IFC(DebugClient->QueryInterface(IID_PPV_ARGS(&DebugRegisters)));


Cleanup:
	return hr;
}



void ReleaseInterfaces()
{
	//can use ReleaseInterface macro for these
	if (DebugControl) { DebugControl->Release(); DebugControl = nullptr; }
	if (DebugSymbols) { DebugSymbols->Release(); DebugSymbols = nullptr; }
	if (DebugDataSpaces) { DebugDataSpaces->Release(); DebugDataSpaces = nullptr; }
	if (DebugRegisters) { DebugRegisters->Release(); DebugRegisters = nullptr; }
}

