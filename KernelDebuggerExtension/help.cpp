#include "stdafx.h"

#define APPNAME _T("KD")

HRESULT CALLBACK help(_In_ PDEBUG_CLIENT DebugClient, _In_opt_ PCSTR args)
{
	HRESULT hr = S_OK;

	PDEBUG_CONTROL4 DebugControl;
	HMODULE hModule;
	TCHAR FileName[MAX_PATH];
	TCHAR Text[MAX_PATH] = APPNAME  _T(" ");  //compiler automatically concatenates two literals next to each other eg. L"KD" L" " => L"KD "
	DWORD Handle;
	PVOID Block;
	DWORD BlockSize;
	PVOID String;
	UINT Length;

	UNREFERENCED_PARAMETER(args);

	IFC(DebugClient->QueryInterface(IID_PPV_ARGS(&DebugControl)));

	hModule = GetModuleHandle(APPNAME);
	IFCNULL(hModule);

	IFC(DebugControl->OutputWide(DEBUG_OUTPUT_NORMAL, _T("\n")));

	IFC(GetModuleFileName(hModule, FileName, MAX_PATH));
	
	BlockSize = GetFileVersionInfoSize(FileName, &Handle);
	IFCNULL(BlockSize);

	//HeapAlloc lets you specify which heap you want to allocate memory. In this case the heap of the calling process.
	Block = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BlockSize);   //GetProcessHeap Retrieves a handle to the default heap of the calling process
	IFCNULL(Block);

	IFC(GetFileVersionInfo(FileName, Handle, BlockSize, Block));
	IFCNULL(Block);

	IFC(VerQueryValue(Block, _T("\\StringFileInfo\\040904b0\\FileVersion"), &String, &Length));
	IFC(StringCchCat(Text, MAX_PATH, reinterpret_cast<wchar_t *> (String)));
	IFC(DebugControl->OutputWide(DEBUG_OUTPUT_NORMAL, Text));
	IFC(DebugControl->OutputWide(DEBUG_OUTPUT_NORMAL, _T("\n")));

	IFC(VerQueryValue(Block, _T("\\StringFileInfo\\040904b0\\LegalCopyRight"), &String, &Length));
	IFC(StringCchCopy(Text, MAX_PATH, reinterpret_cast<wchar_t *> (String)));
	IFC(DebugControl->OutputWide(DEBUG_OUTPUT_NORMAL, Text));
	IFC(DebugControl->OutputWide(DEBUG_OUTPUT_NORMAL, _T("\n\n")));

	IFC(DebugControl->ControlledOutputWide(DEBUG_OUTCTL_ALL_CLIENTS | DEBUG_OUTCTL_DML, DEBUG_OUTPUT_NORMAL,
		_T("help - Displays this list\n")
		_T("st - Displays system service table\n")
		));

Cleanup:
	if (Block) HeapFree(GetProcessHeap(), 0, Block);
	DebugControl->Release();
	return hr;
}