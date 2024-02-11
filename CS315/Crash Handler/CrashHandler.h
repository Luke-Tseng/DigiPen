#pragma once
#include <Windows.h>
#include <dbghelp.h>
#pragma comment ( lib, "dbghelp.lib" )

LONG WINAPI crashHandler(EXCEPTION_POINTERS* exception);
