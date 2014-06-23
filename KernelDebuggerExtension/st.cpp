#include "stdafx.h"
#include "dllmain.h"  //to call QueryInterfaces and ReleaseInterfaces helper methods


//Displays system service table
HRESULT CALLBACK st(_In_ PDEBUG_CLIENT DebugClient, _In_ PCSTR args)
{
	HRESULT hr = S_OK;
	ULONG ProcessorType;
	ULONG PlatformId;
	ULONG Major, Minor, ServicePackNumber;
	ULONG64 KeServiceDescriptorTable;
	ULONG64 KiServiceLimit;
	ULONG64 ServiceTableBase;
	ULONG Limit;
	ULONG64 Address;
	ULONG64 ServiceAddress;
	ULONG i;
	LONG Offset;
	ULONG BytesRead;
	CHAR ServiceName[MAX_PATH];

	UNREFERENCED_PARAMETER(args);

	IFC(QueryInterfaces(DebugClient));

	IFC(DebugControl->GetActualProcessorType(&ProcessorType));
	IFC(DebugControl->GetSystemVersion(&PlatformId, &Major, &Minor, nullptr, NULL, nullptr, &ServicePackNumber, nullptr, NULL, nullptr));

	IFC(DebugSymbols->GetOffsetByName("nt!KeServiceDescriptorTable", &KeServiceDescriptorTable));  //equivalent of x nt!KeServiceDescriptorTable
	IFC(DebugSymbols->GetOffsetByName("nt!KiServiceLimit", &KiServiceLimit));  //equivalent of x nt!KiServiceLimit

Cleanup:
	ReleaseInterfaces();
	return hr;

}
