#include "CrashHandler.h"

LONG WINAPI crashHandler(EXCEPTION_POINTERS* exception)
{
	HANDLE dumpFile = CreateFileA("dump.dmp", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// error check file
	if(dumpFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo = {};
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = exception;
		exceptionInfo.ClientPointers = false;
		MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),dumpFile, static_cast<MINIDUMP_TYPE>(MiniDumpNormal |
		MiniDumpWithDataSegs | MiniDumpWithCodeSegs | MiniDumpWithIndirectlyReferencedMemory |
		MiniDumpWithUnloadedModules | MiniDumpWithFullMemory),&exceptionInfo,NULL,NULL);

		CloseHandle(dumpFile);
	}
	return EXCEPTION_CONTINUE_SEARCH;
}