#pragma once

#define NOMINMAX
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment (lib, "dbghelp.lib")
#include <mmsystem.h>
#pragma comment (lib, "Winmm")
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>

class Profiler
{
public:
	Profiler();
	~Profiler();

	static void StartProfiling();

	static void CALLBACK ThreadCallBackFunction(UINT, UINT, DWORD, DWORD, DWORD);

	static const size_t SAMPLES_TO_RECORD = 10000;

	bool started = false;
	void* mainThread = nullptr;
	unsigned int workerThreadId = 0;
	std::vector<unsigned long long> samples;
};
