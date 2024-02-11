
// BEGIN: DO NOT MODIFY THIS FILE

#include <iostream>
#include "TestScenarios.h"

// Library Prototypes - Provided by project1_scenario_lib.dll
extern "C" void __stdcall project2_scenario7();
extern "C" void __stdcall project2_scenario6();
extern "C" void __stdcall project2_scenario5();
extern "C" void __stdcall project2_scenario4();
extern "C" void __stdcall project2_scenario3();
extern "C" void __stdcall project2_scenario2();
extern "C" void __stdcall project2_scenario1();

static void project2_scenario0() {
	std::cout << "Project 2 - Scenario 0" << std::endl;
}

// Test Harness
void RunTest(int scenario) {
	switch (scenario) {
		case 7:  project2_scenario7(); break;
		case 6:  project2_scenario6(); break;
		case 5:  project2_scenario5(); break;
		case 4:  project2_scenario4(); break;
		case 3:  project2_scenario3(); break;
		case 2:  project2_scenario2(); break;
		case 1:  project2_scenario1(); break;
		default: project2_scenario0(); break;
	}
}
// BEGIN: DO NOT MODIFY THIS FILE
