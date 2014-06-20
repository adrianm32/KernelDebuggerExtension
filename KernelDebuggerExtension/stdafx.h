#pragma once


/*
// Including SDKDDKVer.h defines the highest available Windows platform.

targetver.h and SDKDDKVer.h are used to control what functions, constants, etc. are included into your code from the Windows headers, based on the OS that you want your program to support. I believe that targetver.h sets defaults to using the latest version of Windows unless the defines are specified elsewhere.

SDKDDKVer.h is the header file that actually defines the #defines that represent each version of Windows, IE, etc.

*/
#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN //exclude rarely used stuff from windows headers

#include <Windows.h>
#include <DbgEng.h>   //debug engine header file

#include <tchar.h>

#define IFC(x)    {hr = (x); if (FAILED(hr)){ goto Cleanup;}}
#define IFCOOM(x) if ((x) == NULL) {hr = E_OUTOFMEMORY; goto Cleanup;}
#define IFCPTR(x) if ((x) == NULL) {hr = E_POINTER; goto Cleanup;}
#define IFCW32(x) {hr = HRESULT_FROM_WIN32(x); if (FAILED(hr)){ goto Cleanup;}}
#define IFCEXPECT(x)  if (!(x)) {hr = E_UNEXPECTED; goto Cleanup;}
#define IFCNULL(x) if (x) { hr = S_OK;} else { hr = E_FAIL; goto Cleanup;}
#define IGNOREHR (x) (void)x
