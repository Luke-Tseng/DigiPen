#include "MemDebugger.h"

#if defined(_MSC_VER)
#define NOMINMAX
#include <Windows.h>
#pragma comment(lib, "dbghelp.lib")
#include <DbgHelp.h>

#define GET_CONTEXT(c)         \
	do                         \
	{                          \
		RtlCaptureContext(&c); \
	} while (0)

void FillStackFrame(STACKFRAME64 &stack_frame, const CONTEXT &context)
{
	stack_frame.AddrPC.Mode = AddrModeFlat;
	stack_frame.AddrPC.Offset = context.Rip;
	stack_frame.AddrStack.Mode = AddrModeFlat;
	stack_frame.AddrStack.Offset = context.Rsp;
	stack_frame.AddrFrame.Mode = AddrModeFlat;
	stack_frame.AddrFrame.Offset = context.Rbp;
}

AllocationList allocations;
MemoryDebugger *MemoryDebugger::s_instance = nullptr;
static int s_count;
static MemoryDebugger &memoryInstance = MemoryDebugger::GetInstance();
static MemoryDebuggerInitializer s_logInit;

MemoryDebugger::MemoryDebugger()
{
	SymInitialize(GetCurrentProcess(), nullptr, true);
	SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES);
}

MemoryDebugger::~MemoryDebugger()
{
	if (allocations.empty())
		return;

	LogMemoryLeak();

	// Delete all leaks
	size_t n = allocations.size();
	for (int i = 0; i < n; ++i)
	{
		Deallocate(allocations[0].address, allocations[0].typeFlag);
	}
}

MemoryDebugger &MemoryDebugger::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new MemoryDebugger;
		allocations.pop_back();
	}
	return *s_instance;
}

struct Log
{
	const char *File = "";
	DWORD Line = 0;
	std::string AdditionalInfo = "";
};

void GetLogInfo(void *address, Log &log)
{
	DWORD displacement;
	IMAGEHLP_LINE64 line = {0};
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	// Getting line number and file name of address
	if (SymGetLineFromAddr64(GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &displacement, &line))
	{
		log.Line = line.LineNumber;
		log.File = line.FileName;
	}

	struct
	{
		SYMBOL_INFO symbol_info;
		char buffer[MAX_PATH];
	} symbol = {0};

	symbol.symbol_info.SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol.symbol_info.MaxNameLen = MAX_PATH;
	DWORD64 symbol_offset = 0;
	// Getting function name of the address
	if (SymFromAddr(GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &symbol_offset, &symbol.symbol_info))
	{
		log.AdditionalInfo = symbol.symbol_info.Name;
	}
}

void MemoryDebugger::LogMemoryLeak()
{
	FILE *csvFile = nullptr;
	if (fopen_s(&csvFile, "DebugLog.csv", "a") != 0)
	{
		std::cerr << "Failed to open the CSV file." << std::endl;
		return;
	}
	fseek(csvFile, 0, SEEK_END);
	bool isEmpty = ftell(csvFile) == 0;

	// Write headers
	if (isEmpty)
	{
		fprintf(csvFile, "Message,File,Line,Bytes,Address,Additional Info\n");
	}

	for (const Allocation &a : allocations)
	{
		Log log;
		GetLogInfo(a.returnAddress, log);
		fprintf(csvFile, "Memory Leak,%s,%d,%zu,0x%p,%s\n", log.File, log.Line, a.size, a.address, log.AdditionalInfo.c_str());
	}

	// Close the CSV file
	fclose(csvFile);
}

MemoryDebuggerInitializer::MemoryDebuggerInitializer()
{
	if (++s_count == 1)
	{
		MemoryDebugger::s_instance = static_cast<MemoryDebugger *>(malloc(sizeof(MemoryDebugger)));
		MemoryDebugger::s_instance = new (MemoryDebugger::s_instance) MemoryDebugger;
	}
}

MemoryDebuggerInitializer::~MemoryDebuggerInitializer()
{
	if (--s_count == 0)
	{
		MemoryDebugger::s_instance->~MemoryDebugger();
		free(MemoryDebugger::s_instance);
	}
}

void *GetReturnAddress()
{
	CONTEXT c;
	GET_CONTEXT(c);

	// Fill the initial stack frame information
	STACKFRAME64 stackFrame = {0};
	FillStackFrame(stackFrame, c);

	// Traverse the stack
	int depth = 4;
	for (int i = 0; i < depth; ++i)
	{
		// Find the caller's parent
		BOOL result = StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &stackFrame, &c, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL);

		// Finished or untable to trace further
		if (result == FALSE)
		{
			break;
		}
	}

	return reinterpret_cast<void *>(stackFrame.AddrPC.Offset);
}

void *MemoryDebugger::Allocate(size_t size, int type)
{
	if (size >= std::numeric_limits<size_t>::max() / 2)
	{
		return nullptr;
	}
	const size_t pageSize = 4096;
	size_t pageCount = size / pageSize + 1;
	// Allocating extra page for buffer overflow
	void *basePtr = VirtualAlloc(nullptr, (pageCount + 1) * pageSize, MEM_RESERVE, PAGE_NOACCESS);
	if (!basePtr)
	{
		throw std::bad_alloc();
	}
	// Allocating usable memory
	basePtr = VirtualAlloc(basePtr, pageCount * pageSize, MEM_COMMIT, PAGE_READWRITE);
	if (!basePtr)
	{
		throw std::bad_alloc();
	}
	void *ptr = static_cast<char *>(basePtr) + ((pageCount * pageSize) - size);

	// Creating new allocation info
	Allocation newAllocation;
	newAllocation.address = ptr;
	newAllocation.baseAddress = basePtr;
	newAllocation.size = size;
	newAllocation.typeFlag = type;
	newAllocation.returnAddress = GetReturnAddress();
	allocations.push_back(newAllocation);

	return ptr;
}

void MemoryDebugger::Deallocate(void *address, int type)
{
	if (address == nullptr)
		return;

	for (int i = 0; i < allocations.size(); ++i)
	{
		if (allocations[i].address == address)
		{
			// mismatch
			if (type != allocations[i].typeFlag)
			{
				__debugbreak();
				return;
			}
			// Free address and remove from allocations
			VirtualFree(allocations[i].address, 0, MEM_RELEASE);
			VirtualFree(allocations[i].baseAddress, 0, MEM_DECOMMIT);
			allocations.erase(allocations.begin() + i);
			return;
		}
	}
	// double delete/bad delete
	__debugbreak();
	return;
}

// Overloading new/delete functions
void *operator new(size_t size)
{
	void *ptr = memoryInstance.Allocate(size);
	if (!ptr)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void *operator new(size_t size, const std::nothrow_t &) noexcept
{
	void *ptr = memoryInstance.Allocate(size);
	return ptr;
}

void *operator new[](size_t size)
{
	void *ptr = memoryInstance.Allocate(size, 1);
	if (!ptr)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void *operator new[](size_t size, const std::nothrow_t &) noexcept
{
	void *ptr = memoryInstance.Allocate(size, 1);
	return ptr;
}

void operator delete(void *ptr) noexcept
{
	memoryInstance.Deallocate(ptr);
}

void operator delete(void *ptr, size_t) noexcept
{
	memoryInstance.Deallocate(ptr);
}

void operator delete(void *ptr, const std::nothrow_t &) noexcept
{
	memoryInstance.Deallocate(ptr);
}

void operator delete[](void *ptr) noexcept
{
	memoryInstance.Deallocate(ptr, 1);
}

void operator delete[](void *ptr, size_t) noexcept
{
	memoryInstance.Deallocate(ptr, 1);
}

void operator delete[](void *ptr, const std::nothrow_t &) noexcept
{
	memoryInstance.Deallocate(ptr, 1);
}

#elif defined(__linux__)
#include <execinfo.h>
#include <sys/mman.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <string.h>
#include <link.h>

AllocationList allocations;
MemoryDebugger *MemoryDebugger::s_instance = nullptr;
static int s_count;
static MemoryDebugger &memoryInstance = MemoryDebugger::GetInstance();
static MemoryDebuggerInitializer s_logInit;

MemoryDebugger::MemoryDebugger()
{
}

MemoryDebugger::~MemoryDebugger()
{
	if (allocations.empty())
		return;

	LogMemoryLeak();

	// Delete all leaks
	size_t n = allocations.size();
	for (size_t i = 0; i < n; ++i)
	{
		Deallocate(allocations[0].address, allocations[0].typeFlag);
	}
}

MemoryDebugger &MemoryDebugger::GetInstance()
{
	if (!s_instance)
	{
		s_instance = new MemoryDebugger;
		allocations.pop_back();
	}
	return *s_instance;
}

struct Log
{
	std::string File = "";
	long Line = 0;
	std::string AdditionalInfo = "";
};

size_t ConvertToVMA(size_t addr)
{
	Dl_info info;
	link_map *link_map;
	dladdr1((void *)addr, &info, (void **)&link_map, RTLD_DL_LINKMAP);
	return addr - link_map->l_addr;
}

void GetLogInfo(void *address, Log &log)
{
	Dl_info info;
	if (dladdr(address, &info) && info.dli_sname)
	{
		int result = -1;
		char *demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &result);

		if (result == 0)
		{
			log.AdditionalInfo = demangled;
			free(demangled);
		}
		else
		{
			log.AdditionalInfo = info.dli_sname;
		}

		size_t vma_address = ConvertToVMA(reinterpret_cast<size_t>(address));
		vma_address -= 1;

		char syscom[1024];
		snprintf(syscom, sizeof(syscom), "/usr/bin/addr2line -e %s %zx", info.dli_fname, vma_address);

		FILE *fp = popen(syscom, "r");
		if (!fp)
		{
			std::cerr << "ERROR: Unable to open pipe." << std::endl;
		}

		char path[1024] = {0};
		std::string info;

		while (fgets(path, sizeof(path), fp) != nullptr)
		{
			info += path;
			memset(path, 0, sizeof(path));
		}

		pclose(fp);

		auto pos1 = info.find(':');

		if (pos1 != std::string::npos)
		{
			std::string filePath = info.substr(0, pos1);
			log.File = filePath;
			std::string lineNumStr = info.substr(pos1 + 1, 2);
			log.Line = std::atoi(lineNumStr.c_str());
		}
	}
}

void MemoryDebugger::LogMemoryLeak()
{
	FILE *csvFile = nullptr;
	csvFile = fopen("DebugLog.csv", "a");
	if (!csvFile)
	{
		std::cerr << "Failed to open the CSV file." << std::endl;
		return;
	}
	fseek(csvFile, 0, SEEK_END);
	bool isEmpty = ftell(csvFile) == 0;

	// Write headers
	if (isEmpty)
	{
		fprintf(csvFile, "Message,File,Line,Bytes,Address,Additional Info\n");
	}

	for (const Allocation &a : allocations)
	{
		Log log;
		GetLogInfo(a.returnAddress, log);
		fprintf(csvFile, "Memory Leak,%s,%ld,%zu,%p,%s\n", log.File.c_str(), log.Line, a.size, a.address, log.AdditionalInfo.c_str());
	}

	// Close the CSV file
	fclose(csvFile);
}

MemoryDebuggerInitializer::MemoryDebuggerInitializer()
{
	if (++s_count == 1)
	{
		MemoryDebugger::s_instance = static_cast<MemoryDebugger *>(malloc(sizeof(MemoryDebugger)));
		MemoryDebugger::s_instance = new (MemoryDebugger::s_instance) MemoryDebugger;
	}
}

MemoryDebuggerInitializer::~MemoryDebuggerInitializer()
{
	if (--s_count == 0)
	{
		MemoryDebugger::s_instance->~MemoryDebugger();
		free(MemoryDebugger::s_instance);
	}
}

void *GetReturnAddress()
{
	void *buffer[100];
	int depth = 4;

	if (backtrace(buffer, depth) >= 3)
	{
		return buffer[3];
	}
	else
	{
		return nullptr;
	}
}

void *MemoryDebugger::Allocate(size_t size, int type)
{
	if (size >= std::numeric_limits<size_t>::max() / 2)
	{
		return nullptr;
	}
	const size_t pageSize = 4096;
	size_t pageCount = size / pageSize + 1;
	// Allocating extra page for buffer overflow
	void *basePtr = mmap(nullptr, (pageCount + 1) * pageSize, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (!basePtr)
	{
		throw std::bad_alloc();
	}
	// Allocating usable memory
	int result = mprotect(basePtr, pageCount * pageSize, PROT_READ | PROT_WRITE);
	if (result == -1)
	{
		throw std::bad_alloc();
	}
	void *ptr = static_cast<char *>(basePtr) + ((pageCount * pageSize) - size);

	// Creating new allocation info
	Allocation newAllocation;
	newAllocation.address = ptr;
	newAllocation.baseAddress = basePtr;
	newAllocation.size = size;
	newAllocation.typeFlag = type;
	newAllocation.returnAddress = GetReturnAddress();
	allocations.push_back(newAllocation);

	return ptr;
}

void MemoryDebugger::Deallocate(void *address, int type)
{
	if (address == nullptr)
		return;

	for (size_t i = 0; i < allocations.size(); ++i)
	{
		if (allocations[i].address == address)
		{
			// mismatch
			if (type != allocations[i].typeFlag)
			{
				DEBUG_BREAKPOINT();
				return;
			}
			// Free address and remove from allocations
			munmap(allocations[i].baseAddress, allocations[i].size);
			allocations.erase(allocations.begin() + i);
			return;
		}
	}
	// double delete/bad delete
	DEBUG_BREAKPOINT();
	return;
}

// Overloading new/delete functions
void *operator new(size_t size)
{
	void *ptr = memoryInstance.Allocate(size);
	if (!ptr)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void *operator new(size_t size, const std::nothrow_t &) noexcept
{
	void *ptr = memoryInstance.Allocate(size);
	return ptr;
}

void *operator new[](size_t size)
{
	void *ptr = memoryInstance.Allocate(size, 1);
	if (!ptr)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void *operator new[](size_t size, const std::nothrow_t &) noexcept
{
	void *ptr = memoryInstance.Allocate(size, 1);
	return ptr;
}

void operator delete(void *ptr) noexcept
{
	memoryInstance.Deallocate(ptr);
}

void operator delete(void *ptr, size_t) noexcept
{
	memoryInstance.Deallocate(ptr);
}

void operator delete(void *ptr, const std::nothrow_t &) noexcept
{
	memoryInstance.Deallocate(ptr);
}

void operator delete[](void *ptr) noexcept
{
	memoryInstance.Deallocate(ptr, 1);
}

void operator delete[](void *ptr, size_t) noexcept
{
	memoryInstance.Deallocate(ptr, 1);
}

void operator delete[](void *ptr, const std::nothrow_t &) noexcept
{
	memoryInstance.Deallocate(ptr, 1);
}

#endif