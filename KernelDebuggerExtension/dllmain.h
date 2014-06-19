#pragma once

#include "stdafx.h"

//extern declarations. These will be defined later in cpp file.
extern PDEBUG_CONTROL DebugControl;
extern PDEBUG_SYMBOLS DebugSymbols;
extern PDEBUG_DATA_SPACES DebugDataSpaces;
extern PDEBUG_REGISTERS DebugRegisters;

HRESULT QueryInterfaces(_In_ PDEBUG_CLIENT DebugClient);

void ReleaseInterfaces();