#include "Profiler.h"


Profiler profiler;

Profiler::Profiler()
{
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	if (!SymInitialize(GetCurrentProcess(), NULL, true))
	{
		std::cout << "Profiler initialization failed" << std::endl;
		return;
	}

	samples.reserve(SAMPLES_TO_RECORD);

	mainThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, 0, GetCurrentThreadId());

}

static PSYMBOL_INFO GetSymbol(DWORD64 address, PSYMBOL_INFO buff) {
	PDWORD64 displacement = 0;
	PSYMBOL_INFO symbol = (PSYMBOL_INFO)buff;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;
	SymFromAddr(GetCurrentProcess(), address, displacement, symbol);
	return symbol;
}

Profiler::~Profiler()
{
	// loop through all recorded samples
	// resolve the function name
	// record how many times function was hit

	std::unordered_map<std::string, unsigned int> sampleOccurences;

	for (auto& sample : samples)
	{
		ULONG64 buff[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];

		PSYMBOL_INFO symbol = GetSymbol(sample, (PSYMBOL_INFO)buff);

		std::string name(symbol->Name);

		//check if name exists in map
		sampleOccurences[name]++;
	}

	std::ofstream stream("ProfileReport.csv");
	stream << "Function, Times Called, Percentage" << std::endl;
	for (auto& occrence : sampleOccurences)
	{
		stream << "\"" << occrence.first.c_str() << "\", " << occrence.second << ", " << (float)occrence.second / samples.size() << std::endl;
	}

	stream.close();
}

void Profiler::StartProfiling()
{
	profiler.workerThreadId = timeSetEvent(1, 1, (LPTIMECALLBACK)&ThreadCallBackFunction, 0, TIME_PERIODIC | TIME_CALLBACK_FUNCTION | TIME_KILL_SYNCHRONOUS);
}

void CALLBACK Profiler::ThreadCallBackFunction(UINT, UINT, DWORD, DWORD, DWORD)
{
	if (SuspendThread(profiler.mainThread) != -1)
	{
		CONTEXT context = { 0 };
		context.ContextFlags = WOW64_CONTEXT_i386 | CONTEXT_CONTROL;
		GetThreadContext(profiler.mainThread, &context);
		ResumeThread(profiler.mainThread);
		context.Rip;

		// save context.Rip
		profiler.samples.push_back(context.Rip);

		if (profiler.samples.size() >= profiler.SAMPLES_TO_RECORD)
		{
			timeKillEvent(profiler.workerThreadId);
		}
	}
}

