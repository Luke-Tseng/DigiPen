#ifndef MEMORY_DEBUGGER
#define MEMORY_DEBUGGER
#include <iostream>
#include <new>
#include <vector>
#include "Common.h"

struct Allocation
{
	void* address = nullptr;
	void* baseAddress = nullptr;
	void* returnAddress = nullptr;
	size_t size = 0;
	int typeFlag = 0;
};

typedef std::vector<Allocation, Mallocator<Allocation>> AllocationList;

class MemoryDebugger
{
	friend struct MemoryDebuggerInitializer;
public:
	MemoryDebugger();
	~MemoryDebugger();

	static MemoryDebugger& GetInstance();
	void LogMemoryLeak();
	void* Allocate(size_t size, int type = 0);
	void Deallocate(void* address, int type = 0);
private:
	static MemoryDebugger* s_instance;

};

struct MemoryDebuggerInitializer
{
	MemoryDebuggerInitializer();
	~MemoryDebuggerInitializer();
};

void* operator new(size_t size);
void* operator new(size_t size, const std::nothrow_t&) noexcept;
void* operator new[](size_t size);
void* operator new[](size_t size, const std::nothrow_t&) noexcept;
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, size_t) noexcept;
void operator delete(void* ptr, const std::nothrow_t&) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, size_t) noexcept;
void operator delete[](void* ptr, const std::nothrow_t&) noexcept;

#endif