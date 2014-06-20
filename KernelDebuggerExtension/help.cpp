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



Cleanup:
	DebugControl->Release();
	return hr;
}