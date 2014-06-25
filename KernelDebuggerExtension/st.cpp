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

	IFC(DebugDataSpaces->ReadPointersVirtual(1, KeServiceDescriptorTable, &ServiceTableBase)); // dt nt!KeServiceDescriptortable
	IFC(DebugDataSpaces->ReadVirtual(KiServiceLimit, &Limit, sizeof(Limit), &BytesRead));

	Address = ServiceTableBase;

	IFC(DebugControl->Output(DEBUG_OUTPUT_NORMAL, "\n"));

	if (ProcessorType == IMAGE_FILE_MACHINE_I386)
	{
		for (i = 0; i < Limit; i++, Address += sizeof(ULONG))
		{
			ServiceName[0] = '\0';  //do we need to null terminate here?

			IFC(DebugDataSpaces->ReadPointersVirtual(1, Address, &ServiceAddress));
			IFC(DebugSymbols->GetNameByOffset(ServiceAddress, ServiceName, _countof(ServiceName), &BytesRead, nullptr));  //_countof is same as ARRAY_SIZE	
			IFC(DebugControl->Output(DEBUG_OUTPUT_NORMAL, "%03lx:\t%p\t%s\n", i, ServiceAddress, ServiceName)); //%x unsigned hex int, %p pointer address, %s string
		}

	}
	else if (ProcessorType == IMAGE_FILE_MACHINE_AMD64)
	{

		for  (i = 0; i < Limit; i++, Address += sizeof(ULONG))
		{
			ServiceName[0] = '\0'; //do we need to do this?

			IFC(DebugDataSpaces->ReadVirtual(Address, &Offset, sizeof(Offset), &BytesRead));

			if (Minor < 6000)  //windows xp i think.
			{
				Offset &= 0xF; // only first 4 bits.
			}
			else
			{
				Offset >>= 4; //right shift first 4 bits.
			}

			ServiceAddress = ServiceTableBase + Offset;
			IFC(DebugSymbols->GetNameByOffset(ServiceAddress, ServiceName, _countof(ServiceName), &BytesRead, nullptr));
			IFC(DebugControl->Output(DEBUG_OUTPUT_NORMAL, "%03lx:\t%p\t%s\n", i, ServiceAddress, ServiceName)); //%x unsigned hex int, %p pointer address, %s string
		}
	}

	DebugControl->Output(DEBUG_OUTPUT_NORMAL, "\n");

Cleanup:
	ReleaseInterfaces();
	return hr;

}
