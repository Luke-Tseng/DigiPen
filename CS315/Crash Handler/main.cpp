#include <iostream>
#include "TestScenarios.h"
#include "CrashHandler.h"

// Pass 1-7 to choose the scenario
//   IE: project1.exe 5
int main(int argc, char* argv[]) {
	int scenario = 3; // Or change this line to pick a scenario

	if (argc > 1) {
		scenario = std::atoi(argv[1]);
	}

	// You can modify this file and wrap this call, but must call RunTest(scenario)
	SIZE_T stackSize = 20 * 1024;
	SetThreadStackGuarantee(&stackSize);
	SetUnhandledExceptionFilter(crashHandler);

	RunTest(scenario);

	return 0;
}
